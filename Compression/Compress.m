function [y] = Compress(input_signal, Phi)
%COMPRESS Summary of this function goes here
    y = Phi*input_signal(1,:)';
end

