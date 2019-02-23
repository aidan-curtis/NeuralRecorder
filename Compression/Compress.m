function [y] = Compress(input_signal, Phi)
    %COMPRESS Summary of this function goes here
    size(Phi)
    size(input_signal(1,:)')
    y = mtimes(Phi, input_signal(1,:)');
end

