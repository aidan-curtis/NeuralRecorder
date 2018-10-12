function [decompressed_channel_data] = Decompress(y, mother_wav, Phi)
%DECOMPRESS Summary of this function goes here
    N = size(Phi, 2);
    
    
    decompressed_channel_data = zeros(N);
    
%     psi = zeros(N,N);
%     for ii = 1:N
%         ek = zeros(1,N);
%         ek(ii) = 1;
%         psi(ii, :) = idct(ek)';
%     end

    psi = WavMatLevel(N, log2(N), mother_wav)';
    
    
    Theta = Phi*(psi);
    
    
    % Find good starting point
    s2 = pinv(Theta)*y;

    % Find basis persuit solution
    s1 = l1eq_pd(s2,Theta,Theta',y,5e-3,30); % L1-magic toolbox
    decompressed_channel_data = (psi*s1)';
        
end

