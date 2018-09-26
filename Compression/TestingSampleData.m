figure()
q = pepANA.listOfResults{1}.repeat{1}.data{6}{2}
plot(q)
figure(gcf)
plot(q, 'b')

[u, s, v] = svd(double(q))
plot(v(:, 1), v(:, 2), '.', 'markersize', 10)
hold on;
plot(0,0,'r.', 'markersize', 25)


