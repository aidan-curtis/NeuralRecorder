test_data = data;
mother_wav = 'coif5';
num_samples = 5;
Ms = [10:10:1000];
sinads = zeros(num_samples,length(Ms));
for M = Ms
    for i =[1:num_samples]
        compressed = Compress(test_data(i*1024+1:(i+1)*1024), mother_wav, M);
        sinads(i, M) = CalculateSINAD(compressed, test_data(i*1024+1:(i+1)*1024));
    end
end

%% 

mseb(Ms./1024,[ mean(wav_sinads.haar(:, Ms), 1); mean(wav_sinads.db8(:, Ms),1);  mean(wav_sinads.coif5(:, Ms),1);  mean(wav_sinads.dct(:, Ms),1)], [std(wav_sinads.haar(:, Ms), 1); std(wav_sinads.db8(:, Ms), 1);std(wav_sinads.coif5(:, Ms), 1);std(wav_sinads.dct(:, Ms), 1)])
legend('haar', 'db8', 'coif5', 'dct')
xlabel("Compression")
ylabel ("SINAD (dB)")