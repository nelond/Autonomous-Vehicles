/* daisy chain pseudo code for multiple I2C devices */
/* important registers

0x4D writes 1 byte to be the new address
Range from 0x20 – 0x5E and must be even
0x4B disables terminator. device lets I2C pass through

*/


defaultAddr = wiringPiI2CSetup(0x30)//addr of all devices before modification

//begin setup 2 motors in series

wiringPiI2CWriteReg(defaultAddr,0x4D,0x20);//1st device is now 0x20
device1 = wiringPiI2CSetup(0x20);          //create new file handle for 1st device
wiringPiI2CWriteReg(device1,0x4B);		   //terminator disabled
//data can now pass through device1
wiringPiI2CWriteReg(defaultAddr,0x4D,0x22);//2nd device is now 0x22
device2 = wiringPiI2CSetup(0x22);          //create new file handle for 2nd device
//last device in chain, device2 in this case, always has terminator enabled
//data will pass through terminator disabled devices unless their unique address is used

//end setup 2 motors in series

while(1)
{
	device1_lsb = wiringPiI2CReadReg8(device1,0x40);	 
	device2_lsb = wiringPiI2CReadReg8(device2,0x40);
}



	