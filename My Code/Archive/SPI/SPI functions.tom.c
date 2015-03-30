
// SPI access routines for the MAXQ2000 board
//
// Connect the SPI interface to the following pins:
//
//	SPI signal	Port	J2-pin
//	INT#		P60		54			used by the Check_INT function 
//	MISO		P57		56
//	SCK			P56		58
//	MOSI		P55		60
//	SS#			P54		62
// 

#include <intrinsics.h>             // MAXQ2000 specific stuff
#include <iomaxq200x.h>             // ditto

#define CS_HI PO5 |= 0x10;          // Macros to set the MAXQ2000 SPI CS signal (P54) high and low
#define CS_LO PO5 &= ~0x10;

(init)
SPI_Init();							// set up MAXQ2000 to use its SPI port as a master
EIES1_bit.IT12 = 0;					// MAXQ: 0=pos edge triggered IRQ
PD0=0xFF;							// Set port 0 to outputs
PO0 = 0x00;							// here's how you write the LED bar U11


void SPI_Init(void)
{
// MAXQ2000 SPI port
  CKCN = 0x00;              // system clock divisor is 1
  CS_HI                     // CS# high  
  PD5 |= 0x070;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x080;            // Set SPI input pin (DIN) as input.
  SPICK = 0x00;             // fastest SPI clock--div by 2 
  SPICF = 0x00;             // mode(0,0), 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI
// MAX3420E INT pin is tied to MAXQ2000 P60; make it an input
  PD6 &= ~0x01;             // PD6.0=0 (turn off output)
}


BYTE Check_INT(void)		// returns 0 if nothing pending, nonzero if something pending
{
  if(EIF1_bit.IE12)         // Test the IRQ Flag (P60 pin feeds the Int12 IRQ flipflop)
    {
    EIF1_bit.IE12 = 0;      // It's set--clear it
    return(1);              // show an IRQ is active
    }
  else return(0);           // flag=0: no IRQ active
}

// These routines read and write individual bytes, or burst a string of bytes. 
// They assume the following SPI format:
//	First byte send is 'reg'. For the MAX3420E this is a command byte with format rrrrr0d0.
//	rrrrr is the MAX3420E register, d is direction bit, 1=in, 0=out.
//	For a read (d=1) the next byte or bytes are clocked in using the MOSI pin.
//	For a write (d=0) the next byte or bytes are clocked out using the MISO pin.
//	SS# (slave select) goes low at the beginning and high at the end of the transfer.
//

// Read a register, return its value.
BYTE rreg(BYTE reg)
{
BYTE dum;
  CS_LO
  SPIB = reg<<3;                // reg number w. dir=0 (IN)
  while(SPICN_bit.STBY);        // loop if data still being sent
  dum = SPIB;                   // read and toss the input byte
  SPIB=0x00;                    // data is don't care, we're clocking in MISO bits
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI
  return(SPIB);
}

void wreg(BYTE reg, BYTE dat)
{
  CS_LO                         // Set CS# low
  SPIB = (reg<<3)+2;            // send the register number with the DIR bit (b1) set to WRITE
  while(SPICN_bit.STBY);        // loop if data still being sent
  SPIB = dat;                   // send the data
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI                         // set CS# high
}

void readbytes(BYTE reg, BYTE N, BYTE *p)	// read a string of bytes in a burst. p points to a byte array to store the data.
{
BYTE j;
  CS_LO
  SPIB = reg<<3;            // write bit b1=0 to command a read operation
  while(SPICN_bit.STBY);    // loop if data still being sent
  j = SPIB;                 // NECESSARY TO RE-ENABLE THE INPUT BUFFER in BYTE MODE
  for(j=0; j<N; j++)
    {
    SPIB = 0x00;            // dummy value to get the next read byte
    while(SPICN_bit.STBY);  // loop if data still being received
    *p = SPIB;              // store it in the data array
    p++;                    // bump the pointer
    }
  CS_HI
}
void writebytes(BYTE reg, BYTE N, BYTE *p)	// write a string of bytes in a burst. Data in byte array pointed to by p.
{
BYTE j,wd;
  CS_LO
  SPIB = (reg<<3)+2;        // write bit b1=1 to command a write operation
  while(SPICN_bit.STBY);    // loop if data still being sent
  for(j=0; j<N; j++)
    {
    wd = *p;                // write the array value
    SPIB = wd;
    while(SPICN_bit.STBY);  // loop if data still being received
    p++;                    // bump the pointer
    }
  CS_HI
}
