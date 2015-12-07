%connect Raspberry Pi to Parallax Range Finder
%Will Schoenhals

%Pin 14 is Tx
%Pin 15 is Rx
%Pin 8 is CE0 for this serial port

%enable tips for more info
tips = 0;


ipaddress = '169.254.0.2' ;
username = 'pi';
password = 'raspberry';

%commands DONT USE CHANGE TO CHAR EQUIV DEC
Dec_Range = 'R'; %returns 4 digit value in mm
Bin_Range = 'B'; %returns 4 byte bin value in mm
Repeat = 'L'; %repeated range measurement
Auto_Adjust = 'E'; %adjust for lighting
Reset = 'S'; %reset to default
Info = 'V'; %Print version info
Help = 'H'; %print available commands
Get_Blobs = 'O'; %display coordinate, mass, centroid for blobs
Cal_Range = 'X'; %caliberate camera for range finding
Gray_Pic = 'G'; %8 bits/pixel grayscale@ 160x128
Color_Pic  = 'C'; %16 bits/pixel YUV422 color @ 640x16
Proc_Pic = 'P'; %Same as Color_Pic but with background subtraction



%serial port parameters for Parallax
port = '/dev/ttyAMA0';
baudRate = 115200; %fastest allowed
dataBits = 8;
parity = 'none';
stopBits = 1;


%connect to pi
myPi = raspi(ipaddress,username,password);

%connect to serial port
Parallax = serialdev(myPi,port,baudRate,dataBits,parity,stopBits);
pause(4);%let range finder turn on

%GetRange()---------------------------------------------------------------------------
%write command
write(Parallax,82);
%read 
pause(2);%wait for range finder
range = read(Parallax,16);
%parse out useful data
CharArray = strcat(char(range(9)),char(range(10)),char(range(11)),char(range(12)));    
Distance = str2num(CharArray)

%extra info if tips are enabled
if tips == 1;
    showPins(myPi)
end

