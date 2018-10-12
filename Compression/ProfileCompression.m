test_data = data;
mother_wav = 'db8';
num_samples = 3;
N = 1024;
Ms = [10: 10: 1000];
sinads = zeros(num_samples,length(Ms));
for M = Ms
    for i =[1:num_samples]
        Phi = randn(M, N);
        data_slice = test_data(i*N+1:(i+1)*N);
        compressed = Compress(data_slice, Phi);
        decompressed = Decompress(compressed, mother_wav, Phi);
        sinads(i, M) = CalculateSINAD(decompressed, data_slice);
    end
end

%% 
mseb(Ms./1024,[mean(sinads(:, Ms), 1)], [std(sinads(:, Ms), 1)])

% mseb(Ms./1024,[ mean(wav_sinads.haar(:, Ms), 1); mean(wav_sinads.db8(:, Ms),1);  mean(wav_sinads.coif5(:, Ms),1);  mean(wav_sinads.dct(:, Ms),1)], [std(wav_sinads.haar(:, Ms), 1); std(wav_sinads.db8(:, Ms), 1);std(wav_sinads.coif5(:, Ms), 1);std(wav_sinads.dct(:, Ms), 1)])
legend('haar', 'db8', 'coif5', 'dct')
xlabel("Compression")
ylabel ("SINAD (dB)")