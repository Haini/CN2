clc
clear all
close all

load('host3.mat');
load('landline.mat');
load('satellite.mat');

fig=figure;
subplot(2,2,1);
hold on
grid on
title('Host 3 ping time');
histogram(host3.time_ms,'Normalization','probability');
ylabel('probability');
xlabel('time [ms]');

subplot(2,2,2);
hold on
grid on
title('Landline ping time');
histogram(landline.time_ms,'Normalization','probability');
ylabel('probability');
xlabel('time [ms]');

subplot(2,2,3);
hold on
grid on
title('Satellite ping time');
histogram(satellite.time_ms,'Normalization','probability');
ylabel('probability');
xlabel('time [ms]');

subplot(2,2,4);
hold on
grid on
cdfplot(host3.time_ms);
cdfplot(landline.time_ms);
cdfplot(satellite.time_ms);

legend('Host 3','Landline','Satellite','Location','south');

set(gcf, 'Position',  [0, 0, 800,600])

print(fig, 'ping_times', '-depsc');
