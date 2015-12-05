//example sending values

void SendVals(float x,float y,float theta,float sonar1)

{

  nxtEnableHSPort();               //Get NXT High Speed Port #4 ready
  nxtHS_Mode = hsRawMode;          				//Set to Raw Mode (vs. Master/Slave Mode)
  nxtSetHSBaudRate(9600);          //Xbee Default Speed, 9600 baud


  string packet="";
  string str="";
  ubyte us [50];
  char temp;
	long x1 = (long)(x*100);
  str = (long) x1;
  strcat(packet,str);
  strcat(packet,",");

  long y1 = (long)(y*100);
  str = (long) y1;
  strcat(packet,str);
  strcat(packet,",");

  long theta1 = (long)(theta*100);
  str = (long) theta1;
  strcat(packet,str);
  strcat(packet,",");

   long xsonar1 = (long)(sonar1*100);
  str = (long) xsonar1;
  strcat(packet,str);

  strcat(packet,"\r\n");

	for (int i = 0 ; i< strlen(packet);i++)
	{
		temp= StringGetChar(packet,i);
		us[i]=(ubyte)temp;
	}

	nxtWriteRawHS(&us[0], strlen(packet));        //Send the character (myChar=char to send, 1=length of data)





nxtDisplayTextLine(2, "Length: %d",strlen(packet));

}

task main()
{

	while(1){

		SendVals(1234.12354,2,3,4);
		wait1Msec(1000);
	}

}
