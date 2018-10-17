%{
The code is meant for simulating EEG signal with an Arduino from actual
EEG data. You need to install the Matlab Arduino package to use this 
program.

You need to download the data file first as they are huge. 
Data needs to be first processed to squeeze between 0 and 256 because 
Arduino is limited by 16-bit ADC?
%}

clear a % clear instance in workspace
% a = arduino();

% Load data
len = 60*20000;
data = bload('./ec013.527.dat', [31,len], 0);

% preprocessing of data to make them fit between 0 to 255 for arduino
sample = data(2, 1:200);
if min(sample) < 0
    % make data positive because Arduino can't output negative val
    sample = sample + abs(min(sample)); 
end
range = max(sample) - min(sample);
sample = floor(sample/((range+1)/256));
plot(sample);


% writePWMVoltage works on pins 3, 5, 6, 9, 10, and 11 of Arduino Uno
% writePWMVoltage values go from 0 to 255
% UNTESTED ARDUINO CODE! WILL DO AFTER I GET SALEAE
for i = 1:10000
    val = sample(i);
    writePWMVoltage(a, 'D11', val / 2);
    % may need to delay here 
    % and need to reset arduino PWM frequency as the default is too low
end


clear a 