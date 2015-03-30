#include <intrinsics.h>
#include <iomaxq200x.h>


unsigned int sendSPI(unsigned int spib)
{
  SPIB = spib;                  // Load the data to send
  while(SPICN_bit.STBY);        // Loop until the data has been sent.
  SPICN_bit.SPIC = 0;           // Clear the SPI tansfer complete flag.
  return SPIB;  
}


main()
{
int i;
PD5 |= 0x070;             // Set SPI output pins (CS, SCLK, DOUT) as output.
PD5 &= ~0x080;            // Set SPI input pin (DIN) as input.

SPICK = 0x01;             // Configure SPI for rising edge, sample input
SPICF = 0x00;             //   on inactive edge, 8 bit, divide by 16.
SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
SPICN_bit.SPIEN = 1;      // Enable SPI.



SPICF_bit.CHR = 1;                  // Set SPI to 16-bit mode.
  
//while(1)
//{
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0000);
P05 |= 0x10;             // Set /CS low.
//for (i=0;i<=1000;i++);
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0000);
P05 |= 0x10;             // Set /CS low.


P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0200);
P05 |= 0x10;             // Set /CS low.
//for (i=0;i<=1000;i++);
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0000);
P05 |= 0x10;             // Set /CS low.


//for (i=0;i<=1000;i++);
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x80AA);
P05 |= 0x10;             // Set /CS low.

//for (i=0;i<=1000;i++);
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x8055);
P05 |= 0x10;             // Set /CS low.
//}

while(1){
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0500);
P05 |= 0x10;             // Set /CS low.
//for (i=0;i<=1000;i++);
P05 &= ~0x10;             // Set /CS low.
sendSPI(0x0000);
P05 |= 0x10;             // Set /CS low.
}

//for (i=0;i<=250000;i++);


}
