description for files in this folder

MotorWiringPi
Uses wiringPi to send a PWM to the motor controller. No speed control. 
Proves that wiringPi can actually work. This is where you should start to 
make sure your wiringPi library and MatLab is setup correctly

VariableMotorWiringPi
Uses block from MotorWiringPi. Speed is controlled by user defined variables
instead of a static constant.

MotorAndEncoderTest
What the name implies. It is configured to work out of the box (haha). 48 is the decimal
representation of the device address. 48 is one of many registers. In this case it is
the register for the least significant byte of the encoder. The speed can be modified
through GPIO_Value. 