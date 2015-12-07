#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <softPwm.h>
#include <unistd.h>
#include <math.h>

	int gpio1 = 3; //22;
	int gpio2 = 4; //23;

	int gpio_steering = 23;  //4;
	int gpio_pixyPan =  2; //27; 21 for Rev#1
	int gpio_pixyTilt = 5; //24;
	int gpio_lidarPan = 1; //17;
	int gpio_lidarTilt = 26; //18;


double readEncoder(int fd)
{
	int byte0 = wiringPiI2CReadReg8(fd, 0x41);
	int byte1 = wiringPiI2CReadReg8(fd, 0x40);
	int byte2 = wiringPiI2CReadReg8(fd, 0x43);
	int byte3 = wiringPiI2CReadReg8(fd, 0x42);
	//printf("Encoder = {%d, %d, %d, %d}\r\n", byte0, byte1, byte2, byte3);
	double regVal1 = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
	return regVal1;
}

int changeEncoderAddress(int fd, int newAddress)
{
	wiringPiI2CWriteReg8(fd, 0x4D, (2*newAddress));
	int newFd = wiringPiI2CSetup(newAddress);
	wiringPiI2CWrite(newFd, 0x4B);
	usleep(100000);
	wiringPiI2CWrite(newFd, 0x4A);
	usleep(100000);
	return newFd;
}

const int MAXCONTROL = 20;
const int MINCONTROL = 10;

int setMotorSpeed(int gpio, int motorControl)
{
	int filteredMotorControl = motorControl;
	if (filteredMotorControl > MAXCONTROL)
	{
		filteredMotorControl = MAXCONTROL;
	}
	if (filteredMotorControl < MINCONTROL)
	{
		filteredMotorControl = MINCONTROL;
	}
	printf("FILTERED motor control = %d\r\n",filteredMotorControl);
	softPwmWrite(gpio, filteredMotorControl);
	return 0;
}

int moveForward(double speed, double correction)
{
	printf ( "\n correction : %f -- ",correction);
 
	double motorControl1 = (30 - speed) - correction;
	double motorControl2 = speed - correction;

	if (motorControl1 < 12) motorControl1=12;
	if (motorControl1 > 18) motorControl1=18;
	
	if (motorControl2 > 18) motorControl2=18;
	if (motorControl2 < 12) motorControl2=12;

	printf("MotorControl={%f,%f}\r\n",motorControl1, motorControl2);

	setMotorSpeed(gpio1, (int)round((motorControl1)));
	setMotorSpeed(gpio2, (int)round((motorControl2)));
	return 0;
}
int setHardServo (int no, int  valPercent)
{
	pwmWrite (no,valPercent*1024*15/10000);
	return 0;
}
int initializeServos(int gpio_steering1, int gpio_pixyPan1 ,int gpio_pixyTilt1, int gpio_lidarPan1, int gpio_lidarTilt1)
{
	
	softPwmWrite(gpio_steering1, 15);
	softPwmWrite(gpio_pixyPan1 , 5);
	softPwmWrite(gpio_pixyTilt1, 20);
	setHardServo(gpio_lidarPan1, 50);// 50 percent
	softPwmWrite(gpio_lidarTilt1, 22);

	return 0;
}
int readLidar (int lidFd1)
{
	wiringPiI2CWriteReg8(lidFd1,0x00,0x04);

	while (wiringPiI2CReadReg8(lidFd1,0x01) ==1 )
	{
		printf("Lidar is not ready yet\n");
	}
	usleep(20000);
	int a = wiringPiI2CReadReg8(lidFd1,0x0f);
	int b = wiringPiI2CReadReg8(lidFd1,0x10);

	return b + (a<<8) ;
}

int setServo (int no, int val)
{
	softPwmWrite(no, val);
	return 0;
}


int main(void)
{
/*
	if(wiringPiSetupGpio()==-1)
	return 1;

	int gpio1 = 27;
	softPwmCreate(gpio1, 0, 50);
	usleep(100000);	
	while (1)
	{
		softPwmWrite(gpio1, 10);
		usleep(100000);	
	}

	return 0;
*/
	//if(wiringPiSetupGpio()==-1)
	//return 1;

	const int desiredDistFromWall = 60 ;

	wiringPiSetup();
	pinMode(gpio_steering,PWM_OUTPUT);
	pinMode(gpio_lidarPan,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(375);
	pwmSetRange (1024);
	
	

	softPwmCreate(gpio1, 0, 200);
	softPwmCreate(gpio2, 0, 200);

	
	//softPwmCreate(gpio_pixyPan , 0, 200);
	
	//softPwmCreate(gpio_pixyTilt, 0, 200);

	softPwmCreate(gpio_lidarTilt,0, 200);

	//initializeServos(gpio_steering,gpio_pixyPan,gpio_pixyTilt,gpio_lidarPan,gpio_lidarTilt);
	
	
	int lidFd = wiringPiI2CSetup(0x62);
	int fd = wiringPiI2CSetup(0x30);
	
	int newFd = changeEncoderAddress(fd, 0x10);

	//printf("fd = %d\r\n", newFd);
	double result = readEncoder(newFd);


	int fd2 = wiringPiI2CSetup(0x30);
	int newFd2 = changeEncoderAddress(fd2, 0x11);
	//printf("fd2 = %d\r\n", newFd2);
	double result2 = readEncoder(newFd2);
	


	double dt = 100000;
	double integral = 0;
	double previousError = 0;
	double Kp = 0.01;
	double Ki = 0.0008;
	double Kd = 1;

	double error = 0;
	double derivative = 0;
	double output = 0;
	int lidVal = 0;

	int lidarPanServoInd [3] = {20 , 50, 70};
	int pixyPanServoInd  [3] = {0 ,  5, 10}; 

	//	setServo(gpio_pixyTilt,22);
	//	usleep(30000);
	//	setServo(gpio_pixyTilt,0);

		setServo(gpio_lidarTilt,20);
		usleep(30000);
		setServo(gpio_lidarTilt,0);

		setHardServo(gpio_lidarPan,50);
		
		double avg = desiredDistFromWall;
		int errFromWall=0;

	for (int i = 0; i < 200; i++)
	{

		
		lidVal = readLidar (lidFd);
		
		if (lidVal >0 )
		{ // steering based on lidVal
		
			avg = (lidVal+avg*10.0)/(10.0+1) ;		

		}
		errFromWall = avg-desiredDistFromWall;
		if (errFromWall>5)
			setHardServo(gpio_steering,75);
		else if (errFromWall<-5)
			setHardServo(gpio_steering,25);
		else
			setHardServo(gpio_steering,50);


		printf ("\n lidVal: %d",lidVal) ;
		printf ("\n Avg distance: %f",avg) ;
		//setServo(gpio_steering,i);
		


		result = readEncoder(newFd);
		result2 = readEncoder(newFd2);
		error = result + result2 ; 
		integral += error ; 
		derivative = (error - previousError)/dt ; 

		output = Kp*error + Ki*integral  + Kd * derivative;
		printf ("\n LEFT ENCODER: %f -- RIGHT encoder: %f  -- Error: %f \n", result,result2,error);
		moveForward (17 , output) ;
		usleep(dt);
	}
	
	setHardServo(gpio_steering,0);
	setHardServo(gpio_lidarPan,0);
	return 0;
}
	
	