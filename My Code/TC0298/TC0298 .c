//
//  File........: TC0200.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that tests the ETBEI IER.1
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c
#ifndef TC0298_C
#define TC0298_C


#include <stdio.h>
//#include <stdlib.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"      // RT99 Registers and bits
unsigned char f;
unsigned int err;

void POR(void)
{
  PD7 |= 0x01;  
  UVLO_LO
  testDELAY(5);
  UVLO_HI
  testDELAY(5);  
}  

void IFACE_Init(void)
{
//  CKCN = 0x00;              // system clock divisor is 1
  PD5 |= 0x70;             // Set SPI output pins (CS, SCLK, DOUT) as output.
  PD5 &= ~0x80;            // Set SPI input pin (DIN) as input.
  SPICK = 0x02;             // SPI clock--div by  (0x10 = 16)
// SPICK = 0x10;             // SPI clock--div by  (0x10 = 16)
  SPICF = 0x00;             // mode(0,0), 8 bit data
  SPICN_bit.MSTM = 1;       // Set Q2000 as the master.
  SPICN_bit.SPIEN = 1;      // Enable SPI
  SPICF_bit.CHR = 1;                  // Set SPI to 16-bit mode.
  CS_HI
}

unsigned short int sendBYTE(unsigned short int x )
{
  SPIB = x;                  // Load the data to send
  while(SPICN_bit.STBY);        // Loop until the data has been sent.
  SPIB;
  SPICN_bit.SPIC = 0;           // Clear the SPI tansfer complete flag.
  return SPIB;
}

unsigned short int rreg(unsigned short int reg)
{
  CS_LO
  sendBYTE( 0x000D | (reg<<8) ); // bit7 0=read, bit6-bit0 register address value.
  CS_HI
  CS_LO
  sendBYTE(0x0D0D); // Fill data, discarded by RT99.
  CS_HI
  return SPIB;
}

void wreg(unsigned short int reg, BYTE dat)
{
  CS_LO
  sendBYTE( 0x8000 | (reg<<8) | (WORD)dat );
  CS_HI
}


#pragma vector = 0
__interrupt void interrupt_service_routine(void)
{
  unsigned char val;

  while(1)
    {
  if (EIF0 == 0x01) 
  {
      switch ( rreg( IIR ) & 0x0F )
      {
        case IIR_MODEM_STATUS :
          err = 77;
          rreg( MSR );
          break;
        case IIR_TRANSMIT :
            err = 84;
            //wreg( IER, 0x05 ); 
            wreg( THR, 0x4F );
            //wreg( IER, 0x07 ); 
          break;
        case IIR_RECEIVE :
          err = 82;
          //c = (unsigned char) 
          f = rreg( RBR );
//          if ( ((com->in.tail_index+1) ) != (com->in.head_index ) )
//            com->in.buffer[ com->in.tail_index++ ] =  c ;
          break;
        case IIR_LINE_STATUS :
          err = 76;
/*         val = rreg( LSR );
          switch( val & 0x1E )
          {
            case 0: 
              break;
            case LSR_OE: 
              puts( "OE" );
              break;
            case LSR_PE: 
              puts( "PE" );
              break;
            case LSR_FE: 
              puts( "FE" ); 
              break;
            case LSR_BI: 
              puts( "BI" );
              break;
          } */
          rreg( LSR );
          break;
        case IIR_CHAR_TIMEOUT :
          err = 79;
          rreg( RBR );
          break;
         case 0x01:
          EIF0 = 0x00;
          return;
        case 0x03:
          err = 51;
          EIF0 = 0x00;
          return;
        case 0x05:
          err = 53;
          EIF0 = 0x00;
          return;
        case 0x07:
          err = 55;
          EIF0 = 0x00;
          return;
        case 0x08:
          err = 56;
          EIF0 = 0x00;
          return;
        case 0x09:
          err = 57;
          EIF0 = 0x00;
          return;
        case 0x0B:
          err = 66;
          EIF0 = 0x00;
          return;
        case 0x0D:
          err = 68;
          EIF0 = 0x00;
          return;
        case 0x0F:
          err = 70;
          EIF0 = 0x00;
          return;
        default:
          err = 88;
          EIF0 = 0x00;
          return;
      } // end switch()
  } // end if()
  else return;
    } // end while()
} // end ISR

main()
{
  long speed = 115200L;
  char parity = 'N';
  int data = 8;
  int stopbits = 1; 
  unsigned char lcr_out;
  unsigned char low_divisor;
  unsigned char high_divisor;

  POR();
  IFACE_Init();

//  printf("RCR: %X", rreg(RCR) );

  wreg( IER, 0x00 );
//  rreg( RBR );

  wreg( FCR, YES_FIFO);

  low_divisor = (char) (230400L / speed ) & 0xff;
  high_divisor = (char) ((230400L /  speed ) >> 8);
  wreg( LCR, LCR_DLAB ); // Set DLAB = 1
  wreg( DLL, low_divisor );
  wreg( DLM, high_divisor );
  wreg( LCR, 0 ); // Set DLAB = 0

  if ( parity== 'E' )
    lcr_out = LCR_EVEN_PARITY;
  else if ( parity == 'O' )
    lcr_out = LCR_ODD_PARITY;
  else if ( parity == 'M' )
    lcr_out = LCR_MARK_PARITY;
  else if ( parity == 'S' )
    lcr_out = LCR_SPACE_PARITY;
  else
    lcr_out = LCR_NO_PARITY;

  if ( stopbits == 2 )
    lcr_out |= LCR_2_STOP_BITS;

  if ( data == 6 )
    lcr_out |= LCR_6_DATA_BITS;
  else if ( data == 7 )
    lcr_out |= LCR_7_DATA_BITS;
  else if ( data == 8 )
    lcr_out |= LCR_8_DATA_BITS;

  wreg( LCR, lcr_out ); // Set Byte config

   PD0   = 0x00; // Set port0 as input
   EIES0 = 0x01; // Set up falling edge trigger
   EIE0  = 0x01; // enable interrupt 0
   IMR  |= 0x01; // enable interrupts for module 1

   __enable_interrupt();

   // change me
   wreg( IER, 0x02 );


while(1)
{
  //if(err != 0 )
  //{
  //  putchar(err);
    //err = 0;
 // }
}

} // End of main(void)

#endif
