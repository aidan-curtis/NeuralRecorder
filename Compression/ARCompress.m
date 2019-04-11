function [output] = ARCompress(params,signal)
%ARCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    output = zeros(length(signal), 1);
    for i = [length(params)+1:length(signal)]
        pred = 0;
        for j = [1:length(params)]
           pred = pred+params(j)*signal(i-j);
        end
        output(i) = pred;
    end
end

