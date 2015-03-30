//
//  File........: TC0002.c
//  Authors.....: Tom Tripp
//  Description.:  Transmit a string of characters with interrupts 
//  
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38400 bps
//    WORD:   8, N, 1
//    FIFO:   Yes
//    INTS:   Yes
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0002_C
#define TC0002_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS 0x00 //(IER_RX_DATA /* | IER_LINE_STATUS*/ )

// Which interface? - define only one.
#define IFACE_SPI
//#define IFACE_I2C

//#include <stdio.h>
//#include <stdlib.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation

main()
{
///// Declare Variables Here ///////////////////////

int i = 0;
unsigned char string[] = "Maxim - The quick brown fox came down with the pox.";

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


for( i = 0 ; string[i] != 0 ; i++)
{
    port_putc( string[i], port );
}

testDELAY(14000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
