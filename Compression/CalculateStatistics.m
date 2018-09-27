function [spikeTimes, numClustersDetected] = CalculateStatistics(pathRaw, pathOut)
% Input: 
% This function takes in a path to the txt data
% Output:
% 1.) Gets the times of every detected spike
% 2.) Gets the number of detected clusters
% Returns both of these statistics

load('./Osort_params/paramsIn.mat');
load('./Osort_params/paths.mat');
paths.pathOut = pathOut;
paths.pathRaw = pathRaw;
filesToProcess = size(dir([pathRaw '/*.jpg']),1);
thres = 5;
filesToProcess=2;
normalizationChannels = [];
StandaloneGUI(paths, filesToProcess, thres, normalizationChannels, paramsIn)
end

