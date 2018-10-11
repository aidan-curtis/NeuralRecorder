function [output_data] = Compress(input_signal, mother_wav, M)
%COMPRESS Summary of this function goes here
%   Detailed explanation goes here
    N = 1024;
    channel = 1;
    % Load in the input data
    channel_data = input_signal(channel, :);
    compressed_channel_data = zeros(size(channel_data));
    
    psi = zeros(N,N);
    for ii = 1:N
        ek = zeros(1,N);
        ek(ii) = 1;
        psi(ii, :) = idct(ek)';
    end

%     psi = WavMatLevel(N, log2(N), mother_wav)';
    for index = [1:N:(length(channel_data))]
        x = channel_data(index:(index+N-1))';
        % Random Matrix
        Phi = randn(M,N);

        % Compression
        y = Phi*x;

        % DCT basis
        Theta = Phi*(psi);

        % Find good starting point
        s2 = pinv(Theta)*y;

        % Find basis persuit solution
        s1 = l1eq_pd(s2,Theta,Theta',y,5e-3,30); % L1-magic toolbox
        compressed_channel_data(index:(index+N-1)) = (psi*s1);
    end
    output_data = compressed_channel_data;
end

