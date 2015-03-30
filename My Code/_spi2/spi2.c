/*****************************************************************************

	spi2.c		4-24-00		ott
	Used to test uart mode 0 spi functionality on EZ-USB
	This program will do a constant write and readback loop to an
	25C320 spi EEPROM with EZ-USB as the master
	Pins used:	
			C0 -	RxD0 (Data In)	MISO
			C1 - TxD0 (Clk)			SCLK
			C2 - Chip select		CSB
			A6 - RxD0OUT (Data Out)	MOSI

*****************************************************************************/
//#include "spi2.h"
#include <stdio.h>
#include <stdlib.h>
#include <ezusb.h>
#include <ezregs.h>
#include "RT99reg.h"

#define CSHIGH 0x04
#define CSLOW 0xFB
#define READ_CMD 0x00
#define WRITE_CMD 0x80
#define WRITE_ENABLE 6
#define WRITE_DISABLE 4
#define READ_STATUS 5
#define WRITE_STATUS 1

/////////////////////////////////////// Look up table
code BYTE swap[] = {0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240,8,136,
72,200,40,168,104,232,24,152,88,216,56,184,120,248,4,132,68,196,36,164,100,
228,20,148,84,212,52,180,116,244,12,140,76,204,44,172,108,236,28,156,92,220,
60,188,124,252,2,130,66,194,34,162,98,226,18,146,82,210,50,178,114,242,10,
138,74,202,42,170,106,234,26,154,90,218,58,186,122,250,6,134,70,198,38,166,
102,230,22,150,86,214,54,182,118,246,14,142,78,206,46,174,110,238,30,158,94,
222,62,190,126,254,1,129,65,193,33,161,97,225,17,145,81,209,49,177,113,241,9,
137,73,201,41,169,105,233,25,153,89,217,57,185,121,249,5,133,69,197,37,165,
101,229,21,149,85,213,53,181,117,245,13,141,77,205,45,173,109,237,29,157,93,
221,61,189,125,253,3,131,67,195,35,163,99,227,19,147,83,211,51,179,115,243,11,
139,75,203,43,171,107,235,27,155,91,219,59,187,123,251,7,135,71,199,39,167,
103,231,23,151,87,215,55,183,119,247,15,143,79,207,47,175,111,239,31,159,95,
223,63,191,127,255};
	BYTE t[8];//,x;

/////////////////////////////////////// Prototypes
void wreg (BYTE a, BYTE d);
BYTE rreg (BYTE a);
void enable (void);
BYTE status (void);
void spiwritebyte (BYTE d);
BYTE spireadbyte (void);

main()
{
const BYTE dat = 0x55;
//	WORD a;

	PORTCCFG = 0x03;				//Turn on uart pins rxd0, txd0
	PORTACFG = 0x40;				//Turn on uart pin rxd0out
	OEC = 0x04;						//Make CS# output
	SCON0 = 0x13;					//Mode 0, baud 24/12, enable receive
	CKCON &= 0xF8;					//Set stretch 0
	OUTC |= CSHIGH;					//Turn cs high
//	while(TRUE)
//	{
//wreg(RCR, dat);
//		enable();					//Enable write
//wreg(EFR, EFR_SPI_MODE);
		rreg(0x0D);
		rreg(0x0D);
		rreg(0x0D);
		rreg(0x0D);
		rreg(0x0D);
		rreg(0x0D);
		rreg(0x0D);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
		rreg(RCR);
while(1);

//		while (status() & 1);		//Wait until done
//		t = rreg(RCR);				//Try to read back
//		if (t != d)
//			x=0;					//Test for read back, set breakpoint here
//		a++;
//		d++;
//	}
}

void wreg (BYTE reg, BYTE dat)
{
	OUTC &= CSLOW;						//Turn cs low
	spiwritebyte (WRITE_CMD |  reg);	
	spiwritebyte (dat);
	OUTC |= CSHIGH;						//Turn cs high
}

BYTE rreg (BYTE reg)
{
	BYTE d;

	OUTC &= CSLOW;						//Turn cs low
	spiwritebyte (READ_CMD |  reg);
	spiwritebyte (0x00);
	OUTC |= CSHIGH;						//Turn cs high
	OUTC &= CSLOW;						//Turn cs low
	spiwritebyte (0x0D);
	d = spireadbyte();
	OUTC |= CSHIGH;						//Turn cs high
	return (d);
}

void spiwritebyte (BYTE d)
{
	TI = FALSE;							//Clear flag
	SBUF0 = swap[d];					//Write byte
	while (!TI);						//Wait until done transmitting
}

BYTE spireadbyte (void)
{
	RI = FALSE;							//Clear flag
	while (!RI);						//Wait until done receiving
	return (swap[SBUF0]);				//Return byte
}
