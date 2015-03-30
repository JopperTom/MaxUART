//
//  File........: SPI functions.c
//  Authors.....: Tom Tripp
//  Description.: This is the implementation of SPI functions to test the SPI interface 
//  between the Max Q2000 and the RT99/IF073.
//
//  Date........: December 10, 2004
//
//
//  The connection for the SPI interface to the following pins:
//
//	Q2000 SPI signal  Port	J2-pin ---->  RT99 
//	INT#	          P60	54     ---->  IRQB  P187 
//	MISO		  P57	56     ---->  DOUT  P175
//	SCK		  P56	58     ---->  SCLK  P182
//	MOSI		  P55	60     ---->  DIN   P178
//	SS#		  P54   62     ---->  CSB   P180

//#ifndef SPIFUNCTIONS_C
//#define SPIFUNCTIONS_C

#include <intrinsics.h>             // MAXQ2000 specific stuff
#include <iomaxq200x.h>             // ditto
#include <stdio.h>


__no_init volatile __io unsigned char PO5 @ _M(1,1);   /* Port 5 Output Register */

typedef unsigned char BYTE;
typedef unsigned short WORD;

#define CS_HI PO5 |= 0x10;          // Macros to set the MAXQ2000 SPI CS signal (P54) high and low
#define CS_LO PO5 &= ~0x10;

#define RBR_RT99 0x00
#define THR_RT99 0x00
#define IER_RT99 0x01
#define IIR_RT99 0x02
#define FCR_RT99 0x02
#define LCR_RT99 0x03
#define MCR_RT99 0x04
#define LSR_RT99 0x05
#define MSR_RT99 0x06
#define SCR_RT99 0x07
#define DLL_RT99 0x08
#define DLM_RT99 0x09
#define RCR_RT99 0x0D
#define EFR_RT99 0x0E
#define HCR_RT99 0x1F
#define TST_RT99 0x2F

BYTE REGISTER_RT99[] = { RBR_RT99, THR_RT99, IER_RT99, IIR_RT99, 
                         FCR_RT99, LCR_RT99, MCR_RT99, LSR_RT99, 
                         MSR_RT99, SCR_RT99, DLL_RT99, DLM_RT99, 
                         RCR_RT99, EFR_RT99, HCR_RT99, TST_RT99 };

void SPI_Init(void);
BYTE Check_INT(void);
//void rreg(BYTE reg);
void wreg(BYTE reg, BYTE dat);
void readbytes(BYTE reg, BYTE N, BYTE *p);
void writebytes(BYTE reg, BYTE N, BYTE *p);

void TESTDELAY(int x){int y; for( y=0; y<x ; y++);}

// SPI Send Byte Function
// Purpose: Send one BYTE of data to the RT99 via the SPI interface and wait for xfer to complete.
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable contains any BYTE data to be written to the RT99.
// Postcondition: The argument value is written to the SPI port and the proper amount of delay is
// allowed for the transfer. Invalid returned data is discarded.
unsigned int SENDBYTE(int x){ SPIB = x; while(SPICN_bit.STBY); SPIB;   SPICN_bit.SPIC = 0; return SPIB; }


// SPI Read Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and allows proper delay for the data to be returned from the RT99 in that register address. 
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: SPIB contains the data returned from the RT99.
unsigned int rreg(BYTE reg)
{
  // One WORD (MSB = Command, LSB = Data) is sent to the RT99 to request register value.
  CS_LO
  SENDBYTE( 0x00 | reg ); // bit7 0=read, bit6-bit0 register address value.
  SENDBYTE(0x00); // Fill data, discarded by RT99.
  CS_HI

  // One WORD of fill data which allows RT99 to return register value.
  CS_LO
  SENDBYTE(0x00); // Fill data, discarded by RT99.
  SENDBYTE(0x00);// Fill data, discarded by RT99.
  CS_HI
return SPIB;
}

// SPI Write Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and one BYTE of data and writes a BYTE value to the RT99 register address. 
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: The RT99 register has been written with the data BYTE.
void wreg(BYTE reg, BYTE dat)
{
  CS_LO
// Command BYTE specifying which RT99 register to write to.
  SENDBYTE( 0x80 | reg );
// Data byte to write to register address.
  SENDBYTE( dat );
  CS_HI

//  CS_LO
//  SENDBYTE( 0x00 );
//  SENDBYTE( 0x00 );
//  CS_HI
}

void SPI_Init(void)
{
  CKCN = 0x00;              // system clock divisor is 1
  PD5 |= 0x70;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x80;            // Set SPI input pin (DIN) as input.
  SPICK = 0x00;             // fastest SPI clock--div by 2 
  SPICF = 0x00;             // mode(0,0), 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI
  CS_HI
}



main()
{
  int count = 0;
  int fredd = 0xff;
  SPI_Init();			// set up MAXQ2000 SPI port as a master
/*
   wreg(MCR_RT99, 0xAA);
    rreg(MCR_RT99);
    printf(" \nreg: %X  ",MCR_RT99 );
    printf(" val: %X  ", SPIB );
*/

  while( count <= 15 )
  {
    fredd = rreg( REGISTER_RT99[count] );
//    printf(" \nreg: %X  ",REGISTER_RT99[count] );
    printf(" \nreg: %X  ",count );
    printf(" val: %X  ", SPIB );
    printf(" f: %X  ", fredd );
    count++;
  }

}

//#endif
