//
//  File........: TC1117.c
//  Authors.....: Tom Tripp
//  Description.: Test Framing Error - bit3 of the LSR.
//                The Stop bit LCR.2 of this test will be set to 2 stop bit. Data
//                with 1 stop bit will be sent from test system to see if the 
//                LSR bit3 registers the error.
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   8, N, 2
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Mar. 04, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC1117_C
#define TC1117_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
#define POLLED

// Which interface? - define only one.
#define IFACE_SPI
//#define IFACE_I2C

#include <stdio.h>
#include <stdlib.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation
    unsigned char foo;
    int i;
 
main()
{
///// Declare Variables Here ///////////////////////

   
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'N', 8, 2, NO_FIFO, 0x00 );
///// Your Code Here //////////////////////////////


// Use external source to send serial traffic with 1 stop bit.

while(1)
{
  while( ( (foo = rreg( port->uart_base + LSR )) & LSR_DR ) == 0x00 );
  rreg(port->uart_base + RBR);
  putchar(foo);
//  printf(" %X \n", foo);
}
/*
   // Set Loopback Mode
   wreg( port->uart_base + MCR, MCR_LOOP_BACK );

  for( i = 0 ; i <= 16 ; i++)
  {
    //while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
    wreg( port->uart_base + THR, tx[i] );
  }

  foo = rreg( port->uart_base + LCR );
  wreg( port->uart_base + LCR, foo & 0xE7 );

  for( i = 0 ; i <= 75 ; i++)
  {
 // while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
    err[i] = rreg(port->uart_base + LSR);
  }
  
  for( i = 0 ; i <= 75 ; i++)
  {
    putchar(rx[i]);
    putchar(' ');
    printf(" %X \n",err[i]);
  }
 */
  
/*
tx='J';
//wreg( IER, 0x0F );
    while ( 1 ) 
    {
        tx = getchar();
 
        port_putc( (unsigned char) tx, port );

        if ( tx == 'Q' )
            break;
//        else
//            rx = port_getc( port );
//        if ( rx == tx )
//            putchar( 'Y' );
//        else
//            putchar( 'N' );
//    
    }

*/

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
