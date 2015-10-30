BitBangI2Cv1
-completley untested. Meant to be copy-pasted into 'output' tab of S-function builder. 
Needs: debugging, some way to control data collection. 
Maybe an ENABLE pin? while (enable) {take data}. Uses wiringPi and unistd.h libraries.
BitBangI2Cv2
-still untested. Added ENABLE input. When enable is high, run data routines once every second.
variable 'a' is the output and should be able to be read by other Simulink blocks.