/*
TestRegRW.C
SPI sanity check
*/

#include <intrinsics.h>
#include <iomaxq200x.h>
#include "spi.h"

void main(void)
{
BYTE k,dum,flag;
// Set up the MAXQ2000 SPI port
  CKCN = 0x00;              // system clock divisor is 2
  CS_HI                     // CS# high  
  PD5 |= 0x070;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x080;            // Set SPI input pin (DIN) as input.

  SPICK = 0x00;             // fastest SPI clock--div by 2 
  SPICF = 0x00;             // SPI rising edge, sample input on inactive (neg) edge, 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI port

// initialize MAX3410E
wreg(rPINCTL,(bmFDUPSPI|bmINTLEVEL));   // full duplex SPI, neg level
flag=rreg(rPINCTL);                     // check it
//Reset_MAX3410E(1000);
wreg(rGPIO,0xFF);   // all lites off

do
{
wreg(rGPIO,~0x01);
dum=~rreg(rGPIO);
wreg(rGPIO,~0x02);
dum=~rreg(rGPIO);
wreg(rGPIO,~0x04);
dum=~rreg(rGPIO);
wreg(rGPIO,~0x08);
dum=~rreg(rGPIO);
wreg(rGPIO,0xFF);   // all lites off
}
while(1);
}
/*
while(1)
  {
  for(k=0; k<256; k++)
    {
    wreg(rGPIO,k);
    dum=rreg(rGPIO);
      if (dum != k)
        while(1);
    }
  }
}
*/