// SPIfuncs.c
#include <intrinsics.h>
#include <iomaxq200x.h>
#include "spi.h"

void initIO(void)   // Init MAXQ2000 IO pins 
{
  CKCN = 0x00;              // system clock divisor is 1
  CS_HI                     // CS# high  
  PD5 |= 0x070;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x080;            // Set SPI input pin (DIN) as input.
  SPICK = 0x00;             // fastest SPI clock--div by 2 
  SPICF = 0x00;             // SPI rising edge, sample inputon inactive edge, 8 bit.
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI

// Set up the MAXQ interrupt. 3410 INT pin tied to P60 which is INT12

  PD6 &= ~0x01;            // Set P60 as input (3410 interrupt output)

// NOTE: the interrupt type definition is in spi.h

  #ifdef NEG_LEVEL_INT	    // level active INT pin--check int program polls P60
    wreg(rPINCTL,(bmFDUPSPI|bmINTLEVEL)); // Level is always negative polarity
  #endif
  #ifdef NEG_PULSE_INT	        // negative edge INT--check int routine polls IRQ12 flag	
    wreg(rPINCTL,bmFDUPSPI);    // 3410: INTLEVEL=0, POSINT=0
    EIES1_bit.IT12 = 1;         // MAXQ: 1=neg edge
  #endif
  #ifdef POS_PULSE_INT	        // positive edge IRQ--check int routine polls IRQ12 flag		
    wreg(rPINCTL,(bmFDUPSPI|bmPOSINT));   // 3410: INTLEVEL=0, POSINT=1
    EIES1_bit.IT12 = 0;                   // MAXQ: 0=pos edge
  #endif

// EIE1_bit.EX12 = 1;        // enable this particular interrupt (INT12)
// IMR |= 0x02;             // Enable module 1 IRQS

// configure output port 0--LED bar
  PD0 = 0xFF;               // all outputs
  PO0 = 0x00;               // bargraph lights off
}

BYTE Check_INT(void)		// returns 0 if nothing pending, nonzero if something pending
{
#ifdef NEG_LEVEL_INT		// MAX3410E INT# pin is tied to P6.0 pin
return ~PI6 & 0x01;         // Return nonzero if active
#else                       // It's set to edge-trigger [polarity set in initIO()]
  if(EIF1_bit.IE12 == 1)
    {
    EIF1_bit.IE12 = 0;      // clear the IRQ ff
    return(1);
    }
  else return(0);
#endif
}


void Reset_MAX3410E(WORD time)	
{
WORD k;
SETBIT(rUSBCTL,bmCHIPRES);
for(k=0; k<time; k++);		// a delay
CLRBIT(rUSBCTL,bmCHIPRES);
}

void Ack_Status(void)		// a one-byte way to ack the status stage of a CONTROL transfer
{
//  CS_LO
//  SPIB= (rRevision<<3)+1;       // R18 is revision register, LSB is ACKSTAT bit
//  while(SPICN_bit.STBY);        // loop if data still being sent
//  CS_HI
rregAS(rRevision);
}

// Write a MAX3410E register. Call with reg,dat. 
// 1. Asserts CS#
// 2. Shifts the reg value left 3, adds 0x02 to set the write bit, sends reg number
// 3. Waits for completion, then sends the 'dat' byte
// 4. Deasserts CS#
// NOTE:  since interrupts not used to check for 'transmit complete',
//        don't need to clear the SPIC (SPI transfer complete) bit.
//
void wreg(BYTE reg, BYTE dat)
{
  CS_LO                         // Set CS# low
  SPIB = (reg<<3)+2;            // send the register number with the DIR bit (b1) set to WRITE
  while(SPICN_bit.STBY);        // loop if data still being sent
  SPIB = dat;                   // send the data
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI                         // set CS# high
//  SPICN_bit.SPIC = 0;         // Clear the SPI tansfer complete flag.
}

// Write a MAX3410E register with the "ACK STATUS" bit set in the command byte
void wregAS(BYTE reg, BYTE dat)
{
  CS_LO                         // Set CS# low
  SPIB = (reg<<3)+3;            // reg number with DIR=1 (write) and ACKSTAT=1
  while(SPICN_bit.STBY);        // loop if data still being sent
  SPIB = dat;                   // send the data
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI                         // set CS# high
//  SPICN_bit.SPIC = 0;         // Clear the SPI tansfer complete flag.
}

// Read a register, return its value.

unsigned int rreg(unsigned int reg)
{
BYTE dum;
  CS_LO
  SPIB = reg<<3;                // reg number w. dir=0 (IN)
  while(SPICN_bit.STBY);        // loop if data still being sent
  dum = SPIB;                   // NECESSARY TO RE-ENABLE THE INPUT BUFFER in BYTE MODE
  SPIB=0x00;                    // data is don't care, we're clocking in MISO bits
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI
  return(SPIB);
}

// Read a byte (as rreg), but also set the AckStat bit in the command byte.
BYTE rregAS(BYTE reg)
{
BYTE dum;
  CS_LO
  SPIB = (reg<<3)+1;            // reg number w. dir=0 (IN) and ACKSTAT=1
  while(SPICN_bit.STBY);        // loop if data still being sent
  dum = SPIB;                   // NECESSARY TO RE-ENABLE THE INPUT BUFFER in BYTE MODE
  SPIB=0xFF;                    // data is don't care, we're clocking in MISO bits
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI
  return(SPIB);
}


// Read SPI status byte
BYTE readstat(void)
{
  CS_LO
  SPIB = rFNADDR<<3;            // dummy reg number just to do the read
  while(SPICN_bit.STBY);        // loop if data still being sent
  CS_HI
  return(SPIB);
}

void readbytes(BYTE reg, BYTE N, BYTE *p)
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
void writebytes(BYTE reg, BYTE N, BYTE *p)
{
BYTE j,wd;
  CS_LO
  SPIB = (reg<<3)+2;            // write bit b1=1 to command a write operation
  while(SPICN_bit.STBY);        // loop if data still being sent
  for(j=0; j<N; j++)
    {
    wd = *p;                    // write the array value
    SPIB = wd;
    while(SPICN_bit.STBY);      // loop if data still being received
    p++;                        // bump the pointer
    }
  CS_HI
}

