import time
from scipy import signal
import h5py
import numpy as np
from six import string_types
from collections import defaultdict
from .waveform import WaveformExtractor
from matplotlib import pyplot as plt
from .pca import PCA

from .utils import (Bunch,
					 get_excerpts,
					 chunk_bounds,
					 data_chunk,
					 _concatenate,
					 )

def _to_tuples(x):
	return ((i, j) for (i, j) in x)


def _to_list(x):
	return [(i, j) for (i, j) in x]


def _array_list(arrs):
	out = np.empty((len(arrs),), dtype=np.object)
	out[:] = arrs
	return out

def _keep_spikes(samples, bounds):
	"""Only keep spikes within the bounds `bounds=(start, end)`."""
	start, end = bounds
	return (start <= samples) & (samples <= end)

def _split_spikes(groups, idx=None, **arrs):
	"""Split spike data according to the channel group."""
	# split: {group: {'spike_samples': ..., 'waveforms':, 'masks':}}
	dtypes = {'spike_samples': np.float64,
			  'waveforms': np.float32,
			  'masks': np.float32,
			  }
	groups = np.asarray(groups)
	if idx is not None:
		assert idx.dtype == np.bool
		n_spikes_chunk = np.sum(idx)
		# First, remove the overlapping bands.
		groups = groups[idx]
		arrs_bis = arrs.copy()
		for key, arr in arrs.items():
			arrs_bis[key] = arr[idx]
			assert len(arrs_bis[key]) == n_spikes_chunk
	# Then, split along the group.
	groups_u = np.unique(groups)
	out = {}
	for group in groups_u:
		i = (groups == group)
		out[group] = {}
		for key, arr in arrs_bis.items():
			out[group][key] = _concat(arr[i], dtypes.get(key, None))
	return out


def _concat(arr, dtype=None):
	out = np.array([_[...] for _ in arr], dtype=dtype)
	return out



def connected_components(weak_crossings=None,
						 strong_crossings=None,
						 probe_adjacency_list=None,
						 join_size=None,
						 channels=None):
	"""Find all connected components in binary arrays of threshold crossings.

	Parameters
	----------

	weak_crossings : array
		`(n_samples, n_channels)` array with weak threshold crossings
	strong_crossings : array
		`(n_samples, n_channels)` array with strong threshold crossings
	probe_adjacency_list : dict
		A dict `{channel: [neighbors]}`
	channels : array
		An (n_channels,) array with a list of all non-dead channels
	join_size : int
		The number of samples defining the tolerance in time for
		finding connected components


	Returns
	-------

	A list of lists of pairs `(samp, chan)` of the connected components in
	the 2D array `weak_crossings`, where a pair is adjacent if the samples are
	within `join_size` of each other, and the channels are adjacent in
	`probe_adjacency_list`, the channel graph.

	Note
	----

	The channel mapping assumes that column #i in the data array is channel #i
	in the probe adjacency graph.

	"""

	if probe_adjacency_list is None:
		probe_adjacency_list = {}

	if channels is None:
		channels = []

	# If the channels aren't referenced at all but exist in 'channels', add a
	# trivial self-connection so temporal floodfill will work. If this channel
	# is dead, it should be removed from 'channels'.
	probe_adjacency_list.update({i: {i} for i in channels
								if not probe_adjacency_list.get(i)})

	# Make sure the values are sets.
	probe_adjacency_list = {c: set(cs)
							for c, cs in probe_adjacency_list.items()}

	if strong_crossings is None:
		strong_crossings = weak_crossings

	assert weak_crossings.shape == strong_crossings.shape

	# Set of connected component labels which contain at least one strong
	# node.
	strong_nodes = set()

	n_s, n_ch = weak_crossings.shape
	join_size = int(join_size or 0)

	# An array with the component label for each node in the array
	label_buffer = np.zeros((n_s, n_ch), dtype=np.int32)

	# Component indices, a dictionary with keys the label of the component
	# and values a list of pairs (sample, channel) belonging to that component
	comp_inds = {}

	# mgraph is the channel graph, but with edge node connected to itself
	# because we want to include ourself in the adjacency. Each key of the
	# channel graph (a dictionary) is a node, and the value is a set of nodes
	# which are connected to it by an edge
	mgraph = {}
	for source, targets in probe_adjacency_list.items():
		# we add self connections
		mgraph[source] = targets.union([source])

	# Label of the next component
	c_label = 1

	# For all pairs sample, channel which are nonzero (note that numpy .nonzero
	# returns (all_i_s, all_i_ch), a pair of lists whose values at the
	# corresponding place are the sample, channel pair which is nonzero. The
	# lists are also returned in sorted order, so that i_s is always increasing
	# and i_ch is always increasing for a given value of i_s. izip is an
	# iterator version of the Python zip function, i.e. does the same as zip
	# but quicker. zip(A,B) is a list of all pairs (a,b) with a in A and b in B
	# in order (i.e. (A[0], B[0]), (A[1], B[1]), .... In conclusion, the next
	# line loops through all the samples i_s, and for each sample it loops
	# through all the channels.
	for i_s, i_ch in zip(*weak_crossings.nonzero()):
		# The next two lines iterate through all the neighbours of i_s, i_ch
		# in the graph defined by graph in the case of edges, and
		# j_s from i_s-join_size to i_s.
		for j_s in range(i_s - join_size, i_s + 1):
			# Allow us to leave out a channel from the graph to exclude bad
			# channels
			if i_ch not in mgraph:
				continue
			for j_ch in mgraph[i_ch]:
				# Label of the adjacent element.
				adjlabel = label_buffer[j_s, j_ch]
				# If the adjacent element is nonzero we need to do something.
				if adjlabel:
					curlabel = label_buffer[i_s, i_ch]
					if curlabel == 0:
						# If current element is still zero, we just assign
						# the label of the adjacent element to the current one.
						label_buffer[i_s, i_ch] = adjlabel
						# And add it to the list for the labelled component.
						comp_inds[adjlabel].append((i_s, i_ch))

					elif curlabel != adjlabel:
						# If the current element is unequal to the adjacent
						# one, we merge them by reassigning the elements of the
						# adjacent component to the current one.
						# samps_chans is an array of pairs sample, channel
						# currently assigned to component adjlabel.
						samps_chans = np.array(comp_inds[adjlabel],
											   dtype=np.int32)

						# samps_chans[:, 0] is the sample indices, so this
						# gives only the samp,chan pairs that are within
						# join_size of the current point.
						# TODO: is this the right behaviour? If a component can
						# have a width bigger than join_size I think it isn't!
						samps_chans = samps_chans[i_s - samps_chans[:, 0] <=
												  join_size]

						# Relabel the adjacent samp,chan points with current
						# label.
						samps, chans = samps_chans[:, 0], samps_chans[:, 1]
						label_buffer[samps, chans] = curlabel

						# Add them to the current label list, and remove the
						# adjacent component entirely.
						comp_inds[curlabel].extend(comp_inds.pop(adjlabel))

						# Did not deal with merge condition, now fixed it
						# seems...
						# WARNING: might this "in" incur a performance hit
						# here...?
						if adjlabel in strong_nodes:
							strong_nodes.add(curlabel)
							strong_nodes.remove(adjlabel)

					# NEW: add the current component label to the set of all
					# strong nodes, if the current node is strong.
					if curlabel > 0 and strong_crossings[i_s, i_ch]:
						strong_nodes.add(curlabel)

		if label_buffer[i_s, i_ch] == 0:
			# If nothing is adjacent, we have the beginnings of a new
			# component, # so we label it, create a new list for the new
			# component which is given label c_label,
			# then increase c_label for the next new component afterwards.
			label_buffer[i_s, i_ch] = c_label
			comp_inds[c_label] = [(i_s, i_ch)]
			if strong_crossings[i_s, i_ch]:
				strong_nodes.add(c_label)
			c_label += 1

	# Only return the values, because we don't actually need the labels.
	comps = [comp_inds[key] for key in comp_inds.keys() if key in strong_nodes]
	return comps








class FloodFillDetector(object):
	"""Detect spikes in weak and strong threshold crossings.

	Parameters
	----------

	probe_adjacency_list : dict
		A dict `{channel: [neighbors]}`.
	join_size : int
		The number of samples defining the tolerance in time for
		finding connected components

	Example
	-------

	```python
	det = FloodFillDetector(probe_adjacency_list=...,
							join_size=...)
	components = det(weak_crossings, strong_crossings)
	```

	`components` is a list of `(n, 2)` int arrays with the sample and channel
	for every sample in the component.

	"""
	def __init__(self, probe_adjacency_list=None, join_size=None,
				 channels_per_group=None):
		self._adjacency_list = probe_adjacency_list
		self._join_size = join_size
		self._channels_per_group = channels_per_group

	def __call__(self, weak_crossings=None, strong_crossings=None):
		weak_crossings = np.asarray(weak_crossings, np.bool)
		strong_crossings = np.asarray(strong_crossings, np.bool)
		all_channels = sorted([item for sublist
							  in self._channels_per_group.values()
							  for item in sublist])

		cc = connected_components(weak_crossings=weak_crossings,
								  strong_crossings=strong_crossings,
								  probe_adjacency_list=self._adjacency_list,
								  channels=all_channels,
								  join_size=self._join_size,
								  )
		# cc is a list of list of pairs (sample, channel)
		return [np.array(c) for c in cc]


		
class Thresholder(object):
	"""Threshold traces to detect spikes.

	Parameters
	----------

	mode : str
		`'positive'`, `'negative'`, or `'both'`.
	thresholds : dict
		A `{str: float}` mapping for multiple thresholds (e.g. `weak`
		and `strong`).

	Example
	-------

	```python
	thres = Thresholder('positive', thresholds=(.1, .2))
	crossings = thres(traces)
	```

	"""
	def __init__(self,
				 mode=None,
				 thresholds=None,
				 ):
		assert mode in ('positive', 'negative', 'both')
		if isinstance(thresholds, (float, int, np.ndarray)):
			thresholds = {'default': thresholds}
		if thresholds is None:
			thresholds = {}
		assert isinstance(thresholds, dict)
		self._mode = mode
		self._thresholds = thresholds

	def transform(self, data):
		"""Return `data`, `-data`, or `abs(data)` depending on the mode."""
		if self._mode == 'positive':
			return data
		elif self._mode == 'negative':
			return -data
		elif self._mode == 'both':
			return np.abs(data)

	def detect(self, data_t, threshold=None):
		"""Perform the thresholding operation."""
		# Accept dictionary of thresholds.
		if isinstance(threshold, (list, tuple)):
			return {name: self(data_t, threshold=name)
					for name in threshold}
		# Use the only threshold by default (if there is only one).
		if threshold is None:
			assert len(self._thresholds) == 1
			threshold = list(self._thresholds.keys())[0]
		# Fetch the threshold from its name.


		if isinstance(threshold, string_types):
			assert threshold in self._thresholds
			threshold = self._thresholds[threshold]
		# threshold = float(threshold)
		# Threshold the data.
		return data_t > threshold

	def __call__(self, data, threshold=None):
		# Transform the data according to the mode.
		data_t = self.transform(data)
		return self.detect(data_t, threshold=threshold)

		



class Generator:
	def __init__(self):
		self.data = None
		self.load_chunk_size = 1024*10
		self.data_index = 0
		self.done = False
		self.num_channels = 8
		self.samprate = 20000






	def generate_data_from_mat(self, mat_filename):
		f = h5py.File(mat_filename,'r') 
		data = f.get('data') 
		self.data = np.array(data)
		self.cut_data = self.data[:, list(range(8))]
		self.filtered_data = np.zeros((data.shape[0], 8))
		for ch in range(self.num_channels):
			self.filtered_data[:, ch] = self.apply_filter(self.data[:, ch])

		self.ch_features  = [[]for _ in range(self.num_channels)]

		self.get_spikes(self.filtered_data, self.cut_data)



	def bandpass_filter(self, rate=None, low=None, high=None, order=None):
		"""Butterworth bandpass filter."""
		assert low < high
		assert order >= 1
		return signal.butter(order,
							 (low / (rate / 2.), high / (rate / 2.)),
							 'pass')


	def apply_filter(self, x, axis=0):
		"""Apply a filter to an array."""
		filter = self.bandpass_filter(rate=self.samprate,
								low=500,
								high=3000,
								order=3)
		if isinstance(x, list):
			x = np.asarray(x)
		if x.shape[axis] == 0:
			return x
		b, a = filter
		return signal.filtfilt(b, a, x[:], axis=axis).astype(np.float32)


	def chunk_keys(self, n_samples):
		return [chunk.key for chunk in self.iter_chunks(n_samples)]


	def compute_threshold(self, arr, single_threshold=True, std_factor=None):
		"""Compute the threshold(s) of filtered traces.

		Parameters
		----------

		arr : ndarray
			Filtered traces, shape `(n_samples, n_channels)`.
		single_threshold : bool
			Whether there should be a unique threshold for all channels, or
			one threshold per channel.
		std_factor : float or 2-tuple
			The threshold in unit of signal std. Two values can be specified
			for multiple thresholds (weak and strong).

		Returns
		-------

		thresholds : ndarray
			A `(2,)` or `(2, n_channels)` array with the thresholds.

		"""
		assert arr.ndim == 2
		ns, nc = arr.shape

		assert std_factor is not None
		if isinstance(std_factor, (int, float)):
			std_factor = (std_factor, std_factor)
		assert isinstance(std_factor, (tuple, list))
		assert len(std_factor) == 2
		std_factor = np.array(std_factor)

		if not single_threshold:
			std_factor = std_factor[:, None]

		# Get the median of all samples in all excerpts, on all channels.
		if single_threshold:
			median = np.median(np.abs(arr))
		# Or independently for each channel.
		else:
			median = np.median(np.abs(arr), axis=0)

		# Compute the threshold from the median.


		std = median / .6745
		threshold = std_factor * std
		assert isinstance(threshold, np.ndarray)

		if single_threshold:
			assert threshold.ndim == 1
			assert len(threshold) == 2
		else:
			assert threshold.ndim == 2
			assert threshold.shape == (2, nc)
		return threshold



	def n_chunks(self, n_samples):
		"""Number of chunks."""
		return len(list(self.iter_chunks(n_samples)))

	def iter_chunks(self, n_samples):
		"""Iterate over chunks."""
		chunk_size = int(self.samprate)
		overlap = int(.015 * self.samprate)
		for chunk_idx, bounds in enumerate(chunk_bounds(n_samples, chunk_size,
														overlap=overlap)):
			yield Bunch(bounds=bounds,
						s_start=bounds[0],
						s_end=bounds[1],
						keep_start=bounds[2],
						keep_end=bounds[3],
						keep_bounds=(bounds[2:4]),
						key=bounds[2],
						chunk_idx=chunk_idx,
						)




	def _create_thresholder(self, thresholds=None):
		mode = 'both'
		return Thresholder(mode=mode, thresholds=thresholds)

	def _create_pca(self):
		n_pcs = 2
		return PCA(n_pcs=n_pcs)
		
	def waveform_pcs(self, waveforms, masks):
		"""Compute waveform principal components.

		Returns
		-------

		pcs : array
			An `(n_features, n_samples, n_channels)` array.

		"""
		pca = self._create_pca()
		if waveforms is None or not len(waveforms):
			return
		assert (waveforms.shape[0], waveforms.shape[2]) == masks.shape
		return pca.fit(waveforms, masks)


	def step_pcs(self, w_subset=None, m_subset=None):
		# self._pr.start_step('pca', len(self._groups))
		pcs = {}
		for group in {0:8}:
			# Perform PCA and return the components.
			pcs[group] = self.waveform_pcs(w_subset[group],
										   m_subset[group])
			# self._pr.increment()
		return pcs



	def detect(self, traces_f, thresholds=None):
		"""Detect connected waveform components in filtered traces.

		Parameters
		----------

		traces_f : array
			An `(n_samples, n_channels)` array with the filtered data.
		thresholds : dict
			The weak and strong thresholds.

		Returns
		-------

		components : list
			A list of `(n, 2)` arrays with `sample, channel` pairs.

		"""
		# Threshold the data following the weak and strong thresholds.
		thresholder = self._create_thresholder(thresholds)
		# Transform the filtered data according to the detection mode.
		traces_t = thresholder.transform(traces_f)
		# Compute the threshold crossings.
		weak = thresholder.detect(traces_t, 'weak')
		strong = thresholder.detect(traces_t, 'strong')

		# Find dead channels.
		cpg = {0: list(range(8))}
		live_channels = sorted([item for sublist in cpg.values()
								for item in sublist])
		n_channels = traces_f.shape[1]
		dead_channels = np.setdiff1d(np.arange(n_channels), live_channels)

		# Kill threshold crossings on dead channels.
		weak[:, dead_channels] = 0
		strong[:, dead_channels] = 0

		# Run the detection.
		detector = self._create_detector()
		return detector(weak_crossings=weak,
						strong_crossings=strong)


		

	def step_detect(self, traces=None, thresholds=None):
		n_samples, n_channels = traces.shape
		n_chunks = self.n_chunks(n_samples)

		# Use the recording offsets when dealing with multiple recordings.
		self.recording_offsets = getattr(traces, 'offsets', [0, len(traces)])

		# Pass 1: find the connected components and count the spikes.
		# self._pr.start_step('detect', n_chunks)

		# Dictionary {chunk_key: components}.
		# Every chunk has a unique key: the `keep_start` integer.
		n_spikes_total = 0
		for chunk in self.iter_chunks(n_samples):
			chunk_data = data_chunk(traces, chunk.bounds, with_overlap=True)

			# Apply the filter.
			data_f = self.apply_filter(chunk_data)
			assert data_f.dtype == np.float32
			assert data_f.shape == chunk_data.shape

			# Save the filtered chunk.
			self._store['filtered'+str(chunk.key)]=data_f
			# Detect spikes in the filtered chunk.
			components = self.detect(data_f, thresholds=thresholds)
			self._store['components'+str(chunk.key)]=components	
			# Report progress.
			n_spikes_chunk = len(components)
			n_spikes_total += n_spikes_chunk


		return n_spikes_total



	def _create_extractor(self, thresholds):
		print(thresholds)
		before = 32
		after = 32
		weight_power = 2
		probe_channels = {0: list(range(8))}
		return WaveformExtractor(extract_before=before,
								 extract_after=after,
								 weight_power=weight_power,
								 channels_per_group=probe_channels,
								 thresholds=thresholds,
								 )


	def extract_spikes(self, components, traces_f,
					   thresholds=None, keep_bounds=None, s_start=None):
		"""Extract spikes from connected components.

		Returns a split object.

		Parameters
		----------
		components : list
			List of connected components.
		traces_f : array
			Filtered data.
		thresholds : dict
			The weak and strong thresholds.
		keep_bounds : tuple
			(keep_start, keep_end).
		s_start : 0
			Start of the chunk.

		"""
		n_spikes = len(components)
		if n_spikes == 0:
			return {}

		# Transform the filtered data according to the detection mode.
		thresholder = self._create_thresholder()
		traces_t = thresholder.transform(traces_f)
		# Extract all waveforms.
		extractor = self._create_extractor(thresholds)
		groups, samples, waveforms, masks = zip(*[extractor(component,
															data=traces_f,
															data_t=traces_t,
															)
												  for component in components])

		# Create the return arrays.
		groups = np.array(groups, dtype=np.int32)
		assert groups.shape == (n_spikes,)
		assert groups.dtype == np.int32

		samples = np.array(samples, dtype=np.float64)
		assert samples.shape == (n_spikes,)
		assert samples.dtype == np.float64

		# These are lists of arrays of various shapes (because of various
		# groups).
		waveforms = _array_list(waveforms)
		assert waveforms.shape == (n_spikes,)
		assert waveforms.dtype == np.object

		masks = _array_list(masks)
		assert masks.dtype == np.object
		assert masks.shape == (n_spikes,)

		# Reorder the spikes.
		idx = np.argsort(samples)
		groups = groups[idx]
		samples = samples[idx]
		waveforms = waveforms[idx]
		masks = masks[idx]

		# Remove spikes in the overlapping bands.
		# WARNING: add s_start to spike_samples, because spike_samples
		# is relative to the start of the chunk.
		# It is important to add s_start and not keep_start, because of
		# edge effects between overlapping chunks.
		s_start = s_start or 0
		(keep_start, keep_end) = keep_bounds
		idx = _keep_spikes(samples + s_start, (keep_start, keep_end))

		# Split the data according to the channel groups.
		split = _split_spikes(groups, idx=idx, spike_samples=samples,
							  waveforms=waveforms, masks=masks)
		# split: {group: {'spike_samples': ..., 'waveforms':, 'masks':}}

		# Assert that spike samples are increasing.
		for group in split:
			samples = split[group]['spike_samples']
			if samples is not None:
				assert np.all(np.diff(samples) >= 0)

		return split
		
	def _iter_spikes(self, n_samples, step_spikes=1, thresholds=None):
		"""Iterate over extracted spikes (possibly subset).

		Yield a split dictionary `{group: {'waveforms': ..., ...}}`.

		"""
		for chunk in self.iter_chunks(n_samples):

			# Extract a few components.
			components = self._store['components'+str(chunk.key)]
										  
			if components is None or not len(components):
				yield chunk, {}
				continue

			k = np.clip(step_spikes, 1, len(components))
			components = components[::k]

			# Get the filtered chunk.
			chunk_f = self._store['filtered'+str(chunk.key)]

			# Extract the spikes from the chunk.
			split = self.extract_spikes(components, chunk_f,
										keep_bounds=chunk.keep_bounds,
										s_start=chunk.s_start,
										thresholds=thresholds,
										)

			yield chunk, split

	def step_excerpt(self, n_samples=None,
					 n_spikes_total=None, thresholds=None):
		# self._pr.start_step('excerpt', self.n_chunks(n_samples))
		n_chunks = self.n_chunks(n_samples)
		k = int(n_spikes_total / float(10000))
		w_subset = defaultdict(list)
		m_subset = defaultdict(list)
		n_spikes_total = 0
		for chunk, split in self._iter_spikes(n_samples, step_spikes=k,thresholds=thresholds):
			n_spikes_chunk = 0
			for group, out in split.items():
				w_subset[group].append(out['waveforms'])
				m_subset[group].append(out['masks'])
				assert len(out['masks']) == len(out['waveforms'])
				n_spikes_chunk += len(out['masks'])

			n_spikes_total += n_spikes_chunk
			# self._pr.increment(n_spikes=n_spikes_chunk,
			#                    n_spikes_total=n_spikes_total)
		for group in {0:8}:
			w_subset[group] = _concatenate(w_subset[group])
			m_subset[group] = _concatenate(m_subset[group])

		return w_subset, m_subset


	def _create_detector(self):
		graph = {}
		probe_channels = {0: list(range(8))}
		join_size = 1
		return FloodFillDetector(probe_adjacency_list=graph,
								 join_size=join_size,
								 channels_per_group=probe_channels,
								 )


	def features(self, waveforms, pcs):
		"""Extract features from waveforms.

		Returns
		-------

		features : array
			An `(n_spikes, n_channels, n_features)` array.

		"""
		pca = self._create_pca()
		out = pca.transform(waveforms, pcs=pcs)
		assert out.dtype == np.float32
		return out
		
	def step_extract(self, n_samples=None,
					 pcs=None, thresholds=None):
		# self._pr.start_step('extract', self.n_chunks(n_samples))
		# chunk_counts = defaultdict(dict)  # {group: {key: n_spikes}}.
		# n_spikes_total = 0
		for chunk, split in self._iter_spikes(n_samples, thresholds=thresholds):
			# Delete filtered and components cache files.
			# self._store.delete(name='filtered', chunk_key=chunk.key)
			# self._store.delete(name='components', chunk_key=chunk.key)
			# split: {group: {'spike_samples': ..., 'waveforms':, 'masks':}}
			for group, out in split.items():
				out['features'] = self.features(out['waveforms'], pcs[group])
				# Checking that spikes are increasing.
				spikes = out['spike_samples']
				self._store['spike_samples'+str(chunk.key)] = spikes
				self._store['features'+str(chunk.key)] = out['features']
				self._store['masks'+str(chunk.key)] = out['masks']
				self._store['spike_offset'] = chunk.s_start

				# Filter some things out

				for i in range(len(out['features'])):
					channel = np.argmax(out['masks'][i])
					self.ch_features[channel].append(out['features'][i][channel])

			# n_spikes_total = self._store.spike_counts()
			# n_spikes_chunk = self._store.spike_counts(chunk_key=chunk.key)
			# self._pr.increment(n_spikes_total=n_spikes_total,
			#                    n_spikes=n_spikes_chunk)

	def get_spikes(self, filtered_data, unfiltered_data, single_threshold=False ):
		
		n_samples, n_channels = unfiltered_data.shape

		self._store = {}
		thresholds = self.compute_threshold(filtered_data, std_factor=(2, 4.5))

		self.thresholds = {'weak': thresholds[0], 'strong': thresholds[1]}

		n_spikes_total = self.step_detect(traces=unfiltered_data, thresholds=self.thresholds)
		# Excerpt waveforms.
		w_subset, m_subset = self.step_excerpt(n_samples=n_samples,
											   n_spikes_total=n_spikes_total,
											   thresholds=self.thresholds)

		

		# Compute the PCs.
		pcs = self.step_pcs(w_subset=w_subset, m_subset=m_subset)
		
		self.step_extract(n_samples=n_samples, pcs=pcs, thresholds=self.thresholds)


		print(np.array(self.ch_features[0]))
		plt.scatter(np.array(self.ch_features[0])[0,:],np.array(self.ch_features[0])[1,:])
		plt.show()


	def get_data_chunk(self):
		#When we are streaming in data, the visualizer will call this and this will wait for load_chunk_size data to come in.

		returning_data = self.filtered_data[self.data_index*self.load_chunk_size:(self.data_index+1)*self.load_chunk_size, 0:self.num_channels]
		self.data_index+=1
		if(self.data_index*self.load_chunk_size>self.filtered_data.shape[0]):
			self.done = True
		return returning_data

	def filter_data(self, input_data):
		return input_data
