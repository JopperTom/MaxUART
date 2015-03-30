/*
Modified Demo1.c for simple SPI tests
*/

#include <intrinsics.h>
#include <iomaxq200x.h>
#include "spi.h"

void main(void)
{
BYTE flag;
// Set up the MAXQ2000 SPI port
  CKCN = 0x00;              // system clock divisor is 1
  CS_HI                     // CS# high  
  PD5 |= 0x070;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x080;            // Set SPI input pin (DIN) as input.

  SPICK = 0x00;             // fastest SPI clock--div by 2 
  SPICF = 0x00;             // SPI rising edge, sample input on inactive (neg) edge, 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI port

// initialize MAX3410E
// wreg(rPINCTL,(bmFDUPSPI|bmPOSINT));   // full duplex SPI, pos pulse
wreg(rPINCTL,(bmFDUPSPI|bmINTLEVEL));   // full duplex SPI, neg level
flag=rreg(rPINCTL);                   // check it
//Reset_MAX3410E(1000);

  wreg(rUSBIEN,bmBUSACT);       // enable only this one
  flag=rreg(rUSBIEN);           // check it
  wreg(rUSBCTL,bmCONNECT);	    // VBGATE=0 so CONNECT is not conditional on Vbus present
  wreg(rCPUCTL,bmIE);		    // Enable interrupt pin
  wreg(rGPIO,0xFF);             // lites out
  EIES1_bit.IT12 = 0;           // 0=pos edge, 1=neg edge
  while(1)
  {
//   flag=rreg(rUSBIRQ);
//   flag &= bmBUSACT;
   flag = ~PI6 & 0x01;          // Read the pin
//   flag = EIF1_bit.IE12;        // read the interrupt request flag
  
   if(flag)
      {
      L1_ON
      EIF1_bit.IE12=0;          // clear the MAXQ IRQ bit
      wreg(rUSBIRQ,bmBUSACT);   // clear the MAX3410E IRQ bit
      L1_OFF                    // something the scope can see
      }
  }
}

/*
//#define do16bit 1

#ifdef do16bit
  SPICF = 0x04;             // SPI rising edge, sample inputon inactive edge, 16 bit.
  CS_LO
  SPIB = 0xA0FF;            // send "read GPIO register" command (A0)--second byte is dummy value for read
  while(SPICN_bit.STBY);    // wait for 16 SCKS
  rdat = SPIB;              // THIS SETS RDAT TO 19F0, the correct value
  CS_HI                     
#else
  SPICF = 0x00;             // ditto, 8 bit
  CS_LO
  SPIB = 0xA0;              // Send GPIO register address in read command
  while(SPICN_bit.STBY);    // wait for 8 SCKS
  rdat = SPIB;              // the **magic** solution
  SPIB = 0xFF;              // Send second (dummy) byte to generate 8 read clocks
  while(SPICN_bit.STBY);    // wait for 8 SCKS
  rdat = SPIB;              // THIS SETS RDAT TO 0x0019
  CS_HI
#endif
*/



