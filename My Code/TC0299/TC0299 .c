//
//  File........: TC0299.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that uses the SPI interface to test interrupt driven xfers of characters at 38,400, 
//  8N1 with FIFOs disabled
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c
#ifndef TC0299_C
#define TC0299_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS 0x00

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

    char tx;

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

    // The program stays in this loop until the user hits the
    // Q key.  The loop reads in a character from the
    // keyboard and sends it to the COM port.  It then reads
    // in a character from the COM port, and prints it on the
    // screen.
tx='J';
    while ( 1 ) 
    {
        tx = getchar();
 
        while ( port_putc( (unsigned char) tx, port ) != 0 )
       {
          testDELAY(1000);   
       }


        if ( tx == 'Q' )
            break;
/*        else
            rx = port_getc( port );
        if ( rx == tx )
            putchar( 'Y' );
        else
            putchar( 'N' );
*/    }

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
