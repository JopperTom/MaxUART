//
//  File........: TC0134.c
//  Authors.....: Tom Tripp
//  Description.: Test Odd Parity
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   7, O, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 15, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0134_C
#define TC0134_C

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

main()
{
///// Declare Variables Here ///////////////////////

//    char tx;
    int i;

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'O', 7, 1, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

  for( i = 0 ; i <= 127 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
    wreg( THR, (int)i );
  }
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );




/*
tx='J';
//wreg( IER, 0x0F );
    while ( 1 ) 
    {
        tx = getchar();
 
        port_putc( (unsigned char) tx, port );

//wreg( THR, 'C' );



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
