function [le, re, lidar, lmc, rmc, sc, lpa, ppa, x, y, width, height, color] = TCP_Robot()

t = tcpip('0.0.0.0', 16885, 'NetworkRole', 'server');
fopen(t);

data = fread(t, t.BytesAvailable);
le = data(1)+data(2)*256+data(3)*65536+data(4)*16777216; %4 bytes
re = data(5)+data(6)*256+data(7)*65536+data(8)*16777216; %4 bytes
lmc = data(9); %1 byte
rmc = data(10); %1 byte
lidar = data(11)+data(12)*256; %2 bytes
sc = data(13); %1 byte
lpa = data(14); %1 byte
ppa = data(15); %1 byte
x = data(16)+data(17)*256;
y = data(18)+data(19)*256;
width = data(20)+data(21)*256;
height = data(22)+data(23)*256;
color_code = data(24);
%colors = [[139, 69, 19], [255, 0, 0], [255, 165, 0], [255, 255, 0], [0, 255, 0], [0, 0, 255], [128, 0, 128], [128, 128, 128], [255, 255, 255], [0, 0, 0]];
colors = ['brown', 'red', 'orange', 'yellow', 'green', 'blue', 'violet', 'gray', 'white', 'black', 'no color'];
color = colors(color_code);

