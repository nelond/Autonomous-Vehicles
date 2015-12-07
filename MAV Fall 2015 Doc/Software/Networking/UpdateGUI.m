function UpdateGUI(handle, le, re, lidar, lmc, rmc, sc, lpa, ppa, x, y, width, height, color)

a = get(handle,'Children');
b = a(20);
set(b, 'String', le);
b = a(18);
set(b, 'String', re);
b = a(16);
set(b, 'String', lidar);
b = a(13);
set(b, 'String', lmc);
b = a(11);
set(b, 'String', rmc);
b = a(9);
set(b, 'String', sc);
b = a(7);
set(b, 'String', lpa);
b = a(5);
set(b, 'String', ppa);
b = a(3);
set(b, 'String', color);
b = a(2);
set(b, 'String', ['[' num2str(x) ',' num2str(y) ']:[' num2str(width) ',' num2str(height) ']']);