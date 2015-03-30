//
//  File........: TC0138.c
//  Authors.....: Tom Tripp
//  Description.: Test 2 Stop bits in a 6 bit word.
//  NOTE: If using RealTerm or other terminal program to read the results
//        it is easier to set the program to print out the HEX values, 
//        since characters below 5 bits are composed of mostly escape
//        or non-visible characters.
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   7, N, 2
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 22, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0138_C
#define TC0138_C

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
    port_set( port, 57600L, 'N', 7, 2, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////
//while(1){
  for( i = 0 ; i <= 127 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
    wreg( THR, (int)i );
  }
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
//}
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
