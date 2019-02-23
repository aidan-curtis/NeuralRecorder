test_data = double(data);
mother_wav = 'db8';
num_samples = 3;
N = 1024;
Ms = [1:127];
sinads = zeros(num_samples,length(Ms));
NUM_TRAIN = 2048;
train_data = test_data(:, 1:NUM_TRAIN);

for M = Ms
    for i =[1:num_samples]
%         Phi = randn(M, N);
        data_slice = test_data(:, NUM_TRAIN+i*N+1:NUM_TRAIN+(i+1)*N);
%         compressed = PCACompress(data_slice, coeff);
        
        mu = mean(data_slice);
        [eigenvectors, scores] = pca(data_slice);
        Xhat = scores(:,1:M) * eigenvectors(:,1:M)';
        Xhat = bsxfun(@plus, Xhat, mu);
        
%         decompressed = PCADecompress(compressed, mother_wav, coeff);
        sinads(i, M) = CalculateSINAD(Xhat, data_slice);
    end
end

%% 
% mseb(Ms./1024,[mean(old_sinads(:, Ms), 1)], [std(old_sinads(:, Ms), 1)])
mseb(Ms./127,[mean(sinads(:, Ms), 1)], [std(sinads(:, Ms), 1)])

% mseb(Ms./1024,[ mean(wav_sinads.haar(:, Ms), 1); mean(wav_sinads.db8(:, Ms),1);  mean(wav_sinads.coif5(:, Ms),1);  mean(wav_sinads.dct(:, Ms),1)], [std(wav_sinads.haar(:, Ms), 1); std(wav_sinads.db8(:, Ms), 1);std(wav_sinads.coif5(:, Ms), 1);std(wav_sinads.dct(:, Ms), 1)])
% legend('haar', 'db8', 'coif5', 'dct')
xlabel("Compression")
ylabel ("SINAD (dB)")