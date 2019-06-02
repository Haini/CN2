clc;
clear all;
close all;

param = 'Delay';

load('Audio_speex_Landline_incoming');
load('Audio_speex_Landline_outgoing');
load('Audio_speex_Satellite_incoming');
load('Audio_speex_Satellite_outgoing');

load('Audio_PCMU_Landline_incoming');
load('Audio_PCMU_Landline_outgoing');
load('Audio_PCMU_Satellite_incoming');
load('Audio_PCMU_Satellite_outgoing');

load('Audio_opus_Landline_incoming');
load('Audio_opus_Landline_outgoing');
load('Audio_opus_Satellite_incoming');
load('Audio_opus_Satellite_outgoing');

load('Video_H263-1998_Landline_incoming');
load('Video_H263-1998_Landline_outgoing');
load('Video_H263-1998_Satellite_incoming');
load('Video_H263-1998_Satellite_outgoing');

load('Video_MP4V-ES_Landline_incoming');
load('Video_MP4V-ES_Landline_outgoing');
load('Video_MP4V-ES_Satellite_incoming');
load('Video_MP4V-ES_Satellite_outgoing');

load('Video_VP8_Landline_incoming');
load('Video_VP8_Landline_outgoing');
load('Video_VP8_Satellite_incoming');
load('Video_VP8_Satellite_outgoing');

min_height_audio = min( [height(AudioopusLandlineincoming), ...
                            height(AudioopusSatelliteincoming), ...
                            height(AudioPCMUSatelliteincoming), ...
                            height(AudioPCMUSatelliteincoming), ...
                            height(AudiospeexSatelliteincoming), ...
                            height(AudiospeexSatelliteincoming)]);

fig = figure;
set(gcf, 'Position',  [0, 0, 800,300])
% 
% subplot(1,2,1);
% hold on
% % Landline Audio
% cdfplot(AudiospeexLandlineincoming.(param)(1:min_height_audio));
% cdfplot(AudioopusLandlineincoming.(param)(1:min_height_audio));
% cdfplot(AudioPCMULandlineoutgoing.(param)(1:min_height_audio));
% legend('Speex','Opus','PCMU','Location','southeast');
% print(fig, join(['Audio_Codecs_Landline_',param]), '-depsc');
% 
% subplot(1,2,2);
% hold on
% % Satellite Audio
% cdfplot(AudiospeexSatelliteincoming.(param)(1:min_height_audio));
% cdfplot(AudioopusSatelliteincoming.(param)(1:min_height_audio));
% cdfplot(AudioPCMUSatelliteoutgoing.(param)(1:min_height_audio));
% legend('Speex','Opus','PCMU','Location','southeast');
% print(fig, join(['Audio_Codecs_Satellite_',param]), '-depsc');
% 
subplot(1,2,1);
hold on
% Landline Video
cdfplot(VideoH2631998Landlineoutgoing.(param)(1:min_height_audio));
cdfplot(VideoMP4VESLandlineincoming.(param)(1:min_height_audio));
cdfplot(VideoVP8Landlineincoming.(param)(1:min_height_audio));
legend('H263-1998','MP4V-ES','VP8','Location','southeast');
print(fig, join(['Video_Codecs_Landline_',param]), '-depsc');

subplot(1,2,2);
hold on
% Satellite Video
cdfplot(VideoH2631998Satelliteoutgoing.(param)(1:min_height_audio));
cdfplot(VideoMP4VESSatelliteincoming.(param)(1:min_height_audio));
cdfplot(VideoVP8Satelliteincoming.(param)(1:min_height_audio));
legend('H263-1998','MP4V-ES','VP8','Location','southeast');
print(fig, join(['Video_Codecs_Satellite_',param]), '-depsc');