function [] = data_plot(name, param)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
name_short = strrep(name, "_","");
name_short = strrep(name_short, "-","");

data = load(join([name,'_Landline_incoming.mat']));
landline_incoming = data.(join([name_short,'Landlineincoming'], "")).(param);
data = load(join([name,'_Landline_outgoing.mat']));
landline_outgoing = data.(join([name_short,'Landlineoutgoing'], "")).(param);
data = load(join([name,'_Satellite_incoming.mat']));
satellite_incoming = data.(join([name_short,'Satelliteincoming'], "")).(param);
data = load(join([name,'_Satellite_outgoing.mat']));
satellite_outgoing = data.(join([name_short,'Satelliteoutgoing'], "")).(param);

height_VideoH2631998 = min(length(landline_incoming), length(satellite_incoming));

fig = figure;
subplot(2,3,1);
title(join([param,' incoming']));
hold on
grid on
plot(0:height_VideoH2631998-1, landline_incoming(1:height_VideoH2631998))
plot(0:height_VideoH2631998-1, satellite_incoming(1:height_VideoH2631998))
axis([0 height_VideoH2631998-1 -inf inf]);
xlabel('packets');
ylabel('time (ms)');
legend('<- Landline', '<- Satellite');

subplot(2,3,2);
title(join([param,' incoming']));
hold on
histogram(landline_incoming(1:height_VideoH2631998));
histogram(satellite_incoming(1:height_VideoH2631998));
xlabel('time (ms)');
ylabel('packet count');
legend('<- Landline','<- Satellite');

subplot(2,3,3);
hold on
cdfplot(landline_incoming(1:height_VideoH2631998));
cdfplot(satellite_incoming(1:height_VideoH2631998));
legend('<- Landline','<- Satellite','Location','southeast');

subplot(2,3,4);
title(join([param,' outgoing']));
grid on
hold on
plot(0:height_VideoH2631998-1, landline_outgoing(1:height_VideoH2631998))
plot(0:height_VideoH2631998-1, satellite_outgoing(1:height_VideoH2631998))
xlabel('packets');
ylabel('time (ms)');
axis([0 height_VideoH2631998-1 -inf inf]);
legend('-> Landline', '-> Satellite');

subplot(2,3,5);
title(join([param,' outgoing']));
hold on
histogram(landline_outgoing(1:height_VideoH2631998));
histogram(satellite_outgoing(1:height_VideoH2631998));
xlabel('time (ms)');
ylabel('packet count');
legend('-> Landline','-> Satellite');

subplot(2,3,6);
hold on
cdfplot(landline_outgoing(1:height_VideoH2631998));
cdfplot(satellite_outgoing(1:height_VideoH2631998));
legend('-> Landline','-> Satellite','Location','southeast');

set(gcf, 'Position',  [0, 0, 1400,800])

print(fig, join([name,'_',param]), '-depsc');

end

