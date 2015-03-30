/*****************************************************************************

	spi.c	4-19-00	ott
	Used to test software spi functionality on EZ-USB
	This program will do a constant write and readback loop to an
	25C320 spi EEPROM with EZ-USB as the master
	Pins used:	C0 - Chip select
				C1 - Clk
				C2 - Data OUT from EZ
				C3 - Data In to EZ
	This pins can be changed if you change the bitmasks defines here and in
	spiwrite.a51 and spiread.a51

*****************************************************************************/

#include <ezusb.h>
#include <ezregs.h>

#define CSHIGH 1
#define CSLOW 0xFE
#define CLKLOW 0xFD
#define INLOW 0xFB
#define READ_CMD 3
#define WRITE_CMD 2
#define WRITE_ENABLE 6
#define WRITE_DISABLE 4
#define READ_STATUS 5
#define WRITE_STATUS 1

/////////////////////////////////////// Prototypes
void write_25LC320 (WORD a, BYTE d);
BYTE read_25LC320 (WORD a);
void enable_25LC320 (void);
BYTE status_25LC320 (void);
void spiwritebyte (BYTE d);	//Assembly routine
BYTE spireadbyte (void);	//Assembly routine

main()
{
	BYTE d;
	WORD a;
	BYTE t,x;

	PORTCCFG &= 0xF0;	//Turn off special functions
	OEC |= 0x07;	//C0 = CS*, C1 = CLK, C2 = OUT from EZ, C3 = IN to EZ
	OEC &= 0xF7;
	OUTC |= CSHIGH;	//Turn CS* high
	CKCON &= 0xF8;	//Set stretch 0
	while(TRUE)
	{
		enable_25LC320();	//Enable write
		write_25LC320 (a,d);	//Write byte
		while (status_25LC320() & 1);	//Wait until done
		t = read_25LC320 (a);	//Try to read back
		if (t != d)
			x=0;	//Test for read back, set breakpoint here
		a++;
		d++;
	}
}
void write_25LC320 (WORD a, BYTE d)
{
	OUTC &= INLOW & CLKLOW;	//Make sure signals are low
	OUTC &= CSLOW;	//Turn cs low
	spiwritebyte (WRITE_CMD);	
	spiwritebyte (a >> 8);
	spiwritebyte (a);
	spiwritebyte (d);
	OUTC |= CSHIGH;	//Turn cs high
}

BYTE read_25LC320 (WORD a)
{
	BYTE d;

	OUTC &= INLOW & CLKLOW;	//Make sure signals are low
	OUTC &= CSLOW;	//Turn cs low
	spiwritebyte (READ_CMD);
	spiwritebyte (a >> 8);
	spiwritebyte (a);
	d = spireadbyte();
	OUTC |= CSHIGH;	//Turn cs high
	return (d);
}

void enable_25LC320 (void)
{
	OUTC &= INLOW & CLKLOW;	//Make sure signals are low
	OUTC &= CSLOW;	//Turn cs low
	spiwritebyte (WRITE_ENABLE);
	OUTC |= CSHIGH;	//Turn cs high
}

BYTE status_25LC320 (void)
{
	BYTE d;

	OUTC &= INLOW & CLKLOW;	//Make sure signals are low
	OUTC &= CSLOW;	//Turn cs low
	spiwritebyte (READ_STATUS);
	d = spireadbyte();
	OUTC |= CSHIGH;	//Turn cs high
	return (d);
}

