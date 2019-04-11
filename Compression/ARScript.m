ts =  (double(nkdata.eeg(1,:))-mean(nkdata.eeg(1, :)));
size(ts)

train = ts(1, 1:20000)';
mdl = arima(5,0,0); % 2 the lag order
EstMdl = estimate(mdl,train); % y is your data
params = [];
for i = EstMdl.AR
    params = [params i{1}];
end

orig = ts(1, [1:10000, 20000:40000])';
test_signal = double(orig);
pred = ARCompress(params, test_signal);


plot(orig)
hold on
plot(pred)
figure()
plot(pred-orig)
title('error')

