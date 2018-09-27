function [SINAD] = CalculateSINAD(orig_signal,compressed_signal)
    norm(orig_signal, 2)
    norm(orig_signal-compressed_signal, 2)^2
    hold on;
    plot(orig_signal)
    plot(compressed_signal)
    SINAD =  10*log10(norm(orig_signal, 2)^2/(norm(orig_signal-compressed_signal, 2)^2));
end

