#include <ezusb.h>
#include <ezregs.h>
#include "RT99reg.h"

//#define TNG
#define RT99_ADDR		0x1C


// Simple but it works.
// P20 on
// P21 on
// P22 on
const BYTE xdata reg_addr = 0x0D;
BYTE xdata reg_val;



main()
{
	int i;
	EZUSB_InitI2C(); // Initialize EZ-USB I2C controller

	for(i=0;i<=7;i++)
{
	EZUSB_WriteI2C(RT99_ADDR, 0x01, &reg_addr);
	EZUSB_ReadI2C(RT99_ADDR,0x01,&reg_val);
}
	while(1);
} // End of Main()

