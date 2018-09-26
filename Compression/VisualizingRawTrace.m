% vis_channel = 1;
eight_min = 9600000;
one_min = eight_min/8.0
data = bload('/Volumes/Extra/SpikeData/ec013.527.fil', [31,one_min], 0); 


% firfilter('/Volumes/Extra/SpikeData/ec013.527.dat', '/Volumes/Extra/SpikeData/ec013.527.fil', '31')
% plot(data(1, :))