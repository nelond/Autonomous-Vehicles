/* intialize  */
/* don't do anything for mex file generation */
    # ifndef MATLAB_MEX_FILE
	#include <wiringPi.h> 
	#include <unistd.h> //for nanosleep
    int a = 0;
	delay = 100; //100ns. 
	
	//what is our command?
	deviceADDR = 0b00110000 //48
	regADDR = 0b01000000 //64
	//initialize wiringPi. Pins need to be pulled up!
	//initialize pin 8 as SDA (pin 3 on pi) as input
	//initialize pin 9 as SCL (pin 5 on pi) as input
    int wiringPiSetup();	
	pinMode(9,INPUT);	
	pinMode(8,INPUT);	
	nanosleep(delay);
	
	while (enable == 1)
	{	
		sleep(1); //sleep 1 second
		a = getData();//output an 8 bit integer value
		//error message 1: error with connection
		//error message 2: slave didn't ack, comm error
	}
	
	a = getData();//output an 8 bit integer value
	
	
	//BEGIN main routine---------------------------------------------
	//Writes 2 bytes to slave
	//Returns 1 byte from slave
	int getData()
	{
	encoderVal = 0;
	START();	            //start bit
	WRITE(deviceADDR);		//write device address
	if ( SMACK() == 1); 	//we want an ack
		return 2; 			//error		
	WRITE(regADDR);			//write register address
	if ( SMACK() == 1); 	//we want an ack
		return 2; 			//error
	firstByte = READ();		//read one byte
	MasterACK(0);			//0 indicates ACK. We need 2 more bits
	secondByte = READ();	//Ready last Byte
	STOP();					//stop bit
	encoderVal = firstByte*256 + secondByte;
	return encoderVal; //no error
	}
	//END main routine------------------------------------------------
	
	
	
	
	
	//--------------------------------------------------------
	//begin I2C device functions
	//
	//--------------------------------------------------------
	
	//BEGIN START---hahah-------------------------------------
	int START()
	{
	pinMode(8,INPUT); //release sda to float high
	pinMode(9,INPUT); //release scl to float high
	//test if data is low (indicates error)
	if	( digitalRead(8) == 0 && digitalRead(9) == 0) {
		return 1; //error
	}
	else{
	//end test. begin with START
	digitalWrite(8,0);
	pinMode(8,OUTPUT); //set sda to drive low
	nanosleep(delay);
	return 0;
	}
	}
	//END START--haha--------------------------------------------
	
	//START STOP-hahaha------------------------------------------
	STOP()
	{
		digitalWrite(8,0); //set sda low (not output)
		pinMode(8,OUTPUT); //sda driven low
		nanosleep(delay);
		pinMode(9,INPUT); //scl released to float high
		nanosleep(delay);
		pinMode(8,INPUT) //release sda to be driven high
	}
	//END STOP---------------------------------------------------
	
	//BEGIN WRITE----------------------------------------------
	//args: 8 bit number to be written to slave
	int WRITE(data_out)
	{
		numBits = 8;
		digitalWrite(9,0); //scl = 0. pin dir is still output!
		nanosleep(delay);
		while ( numBits > 0 )
		{
			if ( digitalRead(9) == 0){
				return 1; //error
			}
			//no error then write a 1 or 0
			else{
				if (data_out >= 0b10000000){ //determine bit to be sent
				
					pinMode(9,OUTPUT); //scl driven low
					nanosleep(delay);
					pinMode(8,INPUT): //release sda to float high
					nanosleep(delay);
					pinMode(9,INPUT): //release clock to float high
					nanosleep(delay);
					data_out = data_out - 0b10000000; //shift data_out
					//1 written
				}
				else{
					pinMode(9,OUTPUT); //scl driven low
					nanosleep(delay);
					digitalWrite(8,0); //sda driven low (not output yet)!
					nanosleep(delay);
					pinMode(8,OUTPUT); //sda driven low
					nanosleep(delay);
					pinMode(8,INPUT); //release scl to float high
					nanosleep(delay);
					//0 written				
				}
			data_out = data_out <<= 1; //shift data_out left
			numBits = numBits - 1;
			}
		}
		return 0; //no error
		data_out = 0b00000000; //clear reg
	}
	//END WRITE-------------------------------------------------------
	
	//BEGIN READ------------------------------------------------------
	//returns single byte read from slave
	int READ();
	{
		dataHi = 0;
		dataLo = 0;
		count = 0;
		numBits = 8;
		recievedData = 0b00000000; //clear reg
		digitalWrite(9,0); //scl driven low (not output yet!)
		nanosleep(delay);
		while ( numBits > 0)
		{
			pinMode(9,OUTPUT); //scl driven low
			nanosleep(delay);
			pinMode(8,INPUT); //release sda
			nanosleep(delay);
			pinMode(9,INPUT); //release scl to float high
			nanosleep(delay); //slave responds
			//test scl pin
			if ( digitalRead(9) == 0){
				return 1; //error
			}
			recievedData = recievedData <<= 1; //shift read data. expect MSB first
			
			while (count < 5)//take several readings
			{
				if (digitalRead(8) == 1){ //sda is high
					dataHi = dataHi + 1;
				}
				else{
					dataLo = dataLo + 1;
				}
			count = count + 1;
			}
			
			if ( dataHi > dataLo ){
				recievedData |= 0x01 ; //set bit to 1
			}
			else{
				recievedData &= 0x00 ;//set bit to 0
			}
			dataHi = 0;
			dataLo = 0;
			numBits = numBits - 1; //loop for 8 bits
				
		}
		return recievedData; //return with no error
	}
	//END Read--------------------------------------------------------
	
	//BEGIN SLAVE ACKNOWLEDGE-----------------------------------------
	//returns 4 if error, or 0 for a successful ACK
	int SMACK()
	{
		digitalWrite(9,0); //scl driven low (not output yet!)
		pinMode(9,OUTPUT); //scl driven low
		nanosleep(delay);
		pinMode(8,INPUT); //release sda
		nanosleep(delay);
		//see what is read
		if ( digitalRead(8) ){
			return 1;//error
		}
		else{
			return 0;//successful SMACK
		}
		
		digitalWrite(9,0);
		pinMode(9,1); //drive clk low
	}
	//END SLAVE ACKNOWLEDGE-------------------------------------------	
	
	
	//BEGIN MASTER NO-ACKNOWLEDGE or ACKNOWLEDGE----------------------
	//for some applications. send NACK or ACK to slave
	//args: 1 = NACK, 0 = ACK	
	//no returns
	MasterACK(isAck)
	{
		digitalWrite(9,0);
		pinMode(9,OUTPUT);//scl driven low
		nanosleep(delay);
		//the above part might be redundant...
		digitalWrite(8,isAck); //sda driven high/low
		nanosleep(delay);
		pinMode(8,OUTPUT); //sda driven high/low
		nanosleep(dealy);
		pinMode(9,INPUT);//release scl to float high
		nanosleep(delay); //slave reads our nack/ack
		pinMode(9,OUTPUT); //scl driven low
				
	}
	//END MasterACK----------------------------------------------------
	
	//--------------------------------------------------------
	//END I2C device functions
	//--------------------------------------------------------

    # endif