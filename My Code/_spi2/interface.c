#ifndef INTERFACE_C
#define INTERFACE_C

#include <intrinsics.h>             // MAXQ2000 specific stuff
#include <iomaxq200x.h>             // ditto
#include "interface.h"
#include "RT99reg.h"

void POR(void)
{
  PD7 |= 0x01;  
  UVLO_LO
  testDELAY(5);
  UVLO_HI
  testDELAY(5);  
}  


/////////////////////////////////////////////////////////////////
/////////////////////// SPI Functions ///////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef IFACE_SPI
//  The connection for the SPI interface to the following pins:
//
//	Q2000 SPI signal  Port	J2-pin ---->  RT99 
//	INT#	          P60	54     ---->  IRQB  P187 
//	MISO		  P57	56     ---->  DOUT  P175
//	SCK		  P56	58     ---->  SCLK  P182
//	MOSI		  P55	60     ---->  DIN   P178
//	SS#		  P54   62     ---->  CSB   P180

void IFACE_Init(void)
{
//  CKCN = 0x00;              // system clock divisor is 1
  PD5 |= 0x70;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x80;            // Set SPI input pin (DIN) as input.
  SPICK = 0x02;             // SPI clock--div by  (0x10 = 16)
  SPICF = 0x00;             // mode(0,0), 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI
  SPICF_bit.CHR = 1;                  // Set SPI to 16-bit mode.
  CS_HI
}

// SPI Send Byte Function
// Purpose: Send one BYTE of data to the RT99 via the SPI interface and wait for xfer to complete.
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable contains any BYTE data to be written to the RT99.
// Postcondition: The argument value is written to the SPI port and the proper amount of delay is
// allowed for the transfer. Invalid returned data is discarded.
//void SENDBYTE(int x){ SPIB = x; while(SPICN_bit.STBY); SPIB; }
unsigned short int sendBYTE(unsigned short int x )
{
  SPIB = x;                  // Load the data to send
  while(SPICN_bit.STBY);        // Loop until the data has been sent.
  SPIB;
  SPICN_bit.SPIC = 0;           // Clear the SPI tansfer complete flag.
  return SPIB;
}

// SPI Read Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and allows proper delay for the data to be returned from the RT99 in that register address. 
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: SPIB contains the data returned from the RT99.
unsigned short int rreg(unsigned short int reg)
{
  // One WORD (MSB = Command, LSB = Data) is sent to the RT99 to request register value.
  CS_LO
  sendBYTE( 0x0000 | (reg<<8) ); // bit7 0=read, bit6-bit0 register address value.
//  sendBYTE(0x00); // Fill data, discarded by RT99.
  CS_HI

  // One WORD of fill data which allows RT99 to return register value.
  CS_LO
  sendBYTE(0x0700); // Fill data, discarded by RT99.
//  sendBYTE(0x00);// Fill data, discarded by RT99.
  CS_HI
  return SPIB;
}

// SPI Write Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and one BYTE of data and writes a BYTE value to the RT99 register address. 
// Precondition: The Q2000 must be interface with the RT99 via the SPI pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: The RT99 register has been written with the data BYTE.
void wreg(unsigned short int reg, BYTE dat)
{
  CS_LO
// Command BYTE specifying which RT99 register to write to.
  sendBYTE( 0x8000 | (reg<<8) | (WORD)dat );
// Data byte to write to register address.
//  sendBYTE( dat );
  CS_HI
}

// This ends the SPI Section - Below here is the I2C Section
#endif // IFACE_SPI
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#else

/////////////////////////////////////////////////////////////////
///////////////////////I2C Functions ////////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef IFACE_I2C
// PLACE I2C FUNCTIONS HERE
//  The connection for the I2C interface to the following pins:
//
//	Q2000 signals     Port	J2-pin ---->  RT99 
//	INT#	          P??	54     ---->  IRQB P187 
//	???		  P??	56     ---->  A0   P20
//	???		  P??	58     ---->  A1   P21
//	???		  P??	60     ---->  SCL  P77
//	???		  P??   62     ---->  SDA  P192

//void IFACE_Init(void)
//{
//  CKCN = 0x00;              // system clock divisor is 1
//  PD5 |= 0x70;             // Set SPI output pins (CS, SCLK, DOUT) as output.
//  PD5 &= ~0x80;            // Set SPI input pin (DIN) as input.
//  SPICK = 0x00;             // fastest SPI clock--div by 2 
//  SPICF = 0x00;             // mode(0,0), 8 bit data
//  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
//  SPICN_bit.SPIEN = 1;      // Enable SPI
//  SDA_HI
//  SCL_HI
//}

// I2C Send Byte Function
// Purpose: Send one BYTE of data to the RT99 via the I2C interface and wait for xfer to complete.
// Precondition: The Q2000 must be interface with the RT99 via the I2C pins and the 
// reg variable contains any BYTE data to be written to the RT99.
// Postcondition: The argument value is written to the I2C port and the proper amount of delay is
// allowed for the transfer. Invalid returned data is discarded.

inBYTE()
{			// read byte on i2c bus
    int b;
    for ( b = 0; b < 8; b++)
    {
      clear_flag(in);
      if ( ( data & 0x80) != 0 )
        set_flag(in);
      outBIT();
      rotate_left(data);
    }
    inBIT();
//	SDA_HI;		// be sure SDA is configured as input
}

inBIT()
{
//	SCL_HI;		// clock high
//	BTFSS PORTB, SDA	; test SDA bit
    set_flag(in);
    SCL_HI;
    Delay;

    if ( (SDA_IN & sdaInMask) == 0)
    clear_flag(in);

    SCL_LO;
    Delay;    
    
//	inZERO();
//	inONE();
}

inZERO()
{
//	clear any carry
//	i_byte = i_byte << 1 | 0
	contIN;
}

inONE()
{
//	clear any carry
//      i_byte = (i_byte << 1) | 1
	contIN;
}

contIN()
{
	SCL_LO;		// bring clock low
//	DECFSZ _N, F		// decrement index
	inBIT();
}

///////////////////////////////////////////////////////////

outBYTE()			// send o_byte on I2C bus
{
//	MOVLW .8
//	MOVWF _N
}

outBIT()
{
//	BCF STATUS,C		; clear carry
//	RLF O_BYTE, F		; left shift, most sig bit is now in carry
//	BTFSS STATUS, C		; if one, send a one
	outZERO;
	outONE;
}

outZERO()
{
	SDA_LO;		// SDA at zero
	CLOCK_PULSE;	
	SDA_HI;
	outCONT;
}

outONE()
{
	SDA_HI		// SDA at logic one
	CLOCK_PULSE;
	outCONT;
}

outCONT()
{
//	DECFSZ _N, F		// decrement index
	outBIT;
}

///////////////  LOW LEVEL FUNCTIONS  //////////////////////////
		
void NACK(void)			//bring SDA high and clock
{
	SDA_HI;
	CLOCK_PULSE;
}

void ACK(void)
{
	SDA_LO;
	CLOCK_PULSE;
}

void START(void)
{				
//	SCL_LO; //??
	SDA_HI;
	SCL_HI;     
	SDA_LO;		//bring SDA low while SCL is high
//delay
	SCL_LO;
}

void STOP(void)
{
//	SCL_LO;//??
	SDA_LO;
	SCL_HI;
//delay
	SDA_HI;		// bring SDA high while SCL is high
//	SCL_LO;//?????
}

void CLOCK_PULSE(void)			// SCL momentarily to logic one
{
	SCL_HI;
//delay
	SCL_LO;
}

void SDA_IN(void)			// high impedance by making SDA an input
{
//	BSF STATUS, RP0		// bank 1
//	BSF TRISB, SDA		// make SDA pin an input
//	BCF STATUS, RP0		// back to bank 0
	DELAY_SHORT;
}

void SDA_LO(void)
{
//	BCF PORTB, SDA	
//	BSF STATUS, RP0		// bank 1
//	BCF TRISB, SDA		// make SDA pin an output
//	BCF STATUS, RP0		// back to bank 0
	DELAY_SHORT;
}

void SCL_HI(void)
{
//	BSF STATUS, RP0		// bank 1
//	BSF TRISB, SCL		// make SCL pin an input
//	BCF STATUS, RP0		// back to bank 0
	DELAY_SHORT;
}

void SCL_LO(void)
{
//	BCF PORTB, SCL
//	BSF STATUS, RP0		// bank 1
//	BCF TRISB, SCL		// make SCL pin an output
//	BCF STATUS, RP0		// back to bank 0
	DELAY_SHORT;
}

void DELAY_SHORT(void)		// provides nominal 25 usec delay
{
}

void DELAY_SHORT_1(void)
{
}

void DELAY_LONG(void)		// provide 250 ms delay
{
}

OUTTER()
{
//	MOVLW	.110		// close to 1.0 msec delay when set to .110
//	MOVWF 	LOOP2
}

INNER()
{
//	NOP
//	NOP
//	NOP
//	NOP
//	NOP
//	NOP
//	DECFSZ	LOOP2, F	// decrement and leave result in LOOP2 
//				// skip next statement if zero
	INNER;
//	DECFSZ 	LOOP1, F
	OUTTER;
}

//BYTE sendBYTE(unsigned char x )
//{
//  SPIB = x;                  // Load the data to send
//  while(SPICN_bit.STBY);        // Loop until the data has been sent.
//  SPIB;
//  SPICN_bit.SPIC = 0;           // Clear the SPI tansfer complete flag.
//  return SPIB;
//}

// I2C Read Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and allows proper delay for the data to be returned from the RT99 in that register address. 
// Precondition: The Q2000 must be interface with the RT99 via the I2C pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: SPIB contains the data returned from the RT99.
BYTE rreg(BYTE reg)
{
  // One WORD (MSB = Command, LSB = Data) is sent to the RT99 to request register value.
  outBYTE( 0x00 | reg ); // bit7 0=read, bit6-bit0 register address value.

  // One WORD of fill data which allows RT99 to return register value.
  inBYTE(0x00); // Fill data, discarded by RT99.

  return IIC;
}

// I2C Write Register Function
// Purpose: Takes a BYTE value as an argument that represents a RT99 register address
// and one BYTE of data and writes a BYTE value to the RT99 register address. 
// Precondition: The Q2000 must be interface with the RT99 via the I2C pins and the 
// reg variable must contain a hex value of a valid RT99 register address.
// Postcondition: The RT99 register has been written with the data BYTE.
void wreg(BYTE reg, BYTE dat)
{
// Command BYTE specifying which RT99 register to write to.
  sendBYTE( 0x80 | reg );
// Data byte to write to register address.
  sendBYTE( dat );
}
#endif // IFACE_I2C
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


#endif // INTERFACE_C
