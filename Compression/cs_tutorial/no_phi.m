N = 1024;
M = 500;
x = data(1, 1: 1024)';

Psi = WavMatLevel(N, log2(N), 'db8');
Psi_inv = pinv(Psi);
X = Psi*x;

x_m = zeros(N,1);
q = randperm(N);
q = q(1:M);
x_m(q) = x(q);
subplot(5,1,3); plot(real(x_m)); xlim([1 N]);
title('Measured samples of $\mathbf{x(t)}$', 'Interpreter', 'latex');

A = Psi_inv(q, :);      % sensing matrix
y = A*X;                % measured values

% Perform Compressed Sensing recovery
x0 = A.'*y;
X_hat = l1eq_pd(x0, A, [], y, 1e-5);

subplot(5,1,4); plot(abs(X_hat)); xlim([1 N]);
title('$|\mathbf{\hat{X}(f)}|$', 'Interpreter', 'latex');

x_hat = real(Psi_inv*X_hat);    % IFFT of X_hat(f)

subplot(5,1,5); plot(x_hat); xlim([1 N]);  ylim([-amp amp]);
title('$\mathbf{\hat{x}(t)}$', 'Interpreter', 'latex');