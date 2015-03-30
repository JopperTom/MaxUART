/* zapbig.c  8-22-00   LTH
Write the hex bytes [B0 47 05 80 00 01 00] to an EEPROM at address 1.
This is handy for over-writing an EEPROM that was previously loaded with 'B2'
code for boot-load testing. By writing the seven bytes above we create an EEPROM
that acts like the 24LC00 supplied on the stock development board.

To use it:
1.	Rocker Switch 7 [EEPROM A0=1] must be in the down (OFF) position PERMANENTLY.
2.	Flip Rocker Switch 8 [A1] to the down (OFF) position. This sets the EEPROM address to 3.
3.	Press the RESET button. The AN2131 finds 'missing' EEPROM (nothing at address
	0 or 1), and enumerates as generic, using internal VID/PID/DID.
4.	Flip Rocker Switch 8 back to the up (ON) position. The EEPROM is now at address 1.
5.	Start the control panel. It binds to the board because of internal VID/PID/DID.
	The green MONITOR light should come on. You're in the debug environment.	
6.	At any later time re-program the EEPROM by selecting the 'EEPROM' button and 
	choosing an 'iic' file.
7.	When you're finished testing the EEPROM download of code, go back to step 1.
*/
xdata char I2CS	  		_at_ 0x7FA5;	// I2C port Control/Status
xdata char I2DAT  		_at_ 0x7fA6;    // I2C data
#define bSTOPBIT	0x40				// I2CS.6 is i2c STOP bit
void display_hex (char val);			// display 'val' as hex in 7-seg readout

main()
{
	display_hex(0xFE);	// Light up the top segment to show we're running
//
	while (I2CS & bSTOPBIT);	// wait for STOP bit LOW--last operation complete
	I2CS = 0x80;				// set the START bit
	I2DAT = 0xA2;				// EEPROM address 1, b0=0 means 'write'
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1 (i2c transmit complete)
	I2DAT = 0;					// send Addr-H
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0;					// send Addr-L
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0xB0;				// byte 1
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x47;				// byte 2
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x05;				// byte 3
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x80;				// byte 4
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x00;				// byte 5
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x01;				// byte 6
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2DAT = 0x00;				// byte 7
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2CS = 0x40;				// set the STOP bit
	display_hex(0xF7);			// Light up bottom segment
}
	
void display_hex (char val)	
{
	while (I2CS & bSTOPBIT);	// wait for STOP bit LOW--last operation complete
	I2CS = 0x80;				// set the START bit
	I2DAT = 0x42;				// IO expander address=42, LSB=0 for write
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1 (i2c transmit complete)
	I2DAT = val;				// send the data byte
	while ((I2CS&0x01)!=0x01);	// wait for DONE=1
	I2CS = 0x40;				// set the STOP bit
}

