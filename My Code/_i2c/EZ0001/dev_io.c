#include <ezusb.h>
#include <ezregs.h>
#include "RT99reg.h"

//#define TNG
#define RT99_ADDR		0x1C

// P20 on
// P21 on
// P22 on
WORD xdata reg_dat = 0x0722;
BYTE xdata reg_val;
const BYTE xdata reg_addr = 0x07;
const BYTE xdata *p_data = &reg_dat;


main()
{
	EZUSB_InitI2C(); // Initialize EZ-USB I2C controller
	
	// Read the register value
	EZUSB_WriteI2C(RT99_ADDR, 0x01, &reg_addr);
	EZUSB_ReadI2C(RT99_ADDR,0x01,&reg_val);


	// Write a value to the register
	EZUSB_WriteI2C(RT99_ADDR, 0x02, p_data);
//	EZUSB_ReadI2C(RT99_ADDR,0x01,&reg_val);


	// Read new value from register
	EZUSB_WriteI2C(RT99_ADDR, 0x01, &reg_addr);
	EZUSB_ReadI2C(RT99_ADDR,0x01,&reg_val);





while(1);
} // End of Main()



