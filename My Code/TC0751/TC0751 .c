//
//  File........: TC0751.c
//  Authors.....: Tom Tripp
//  Description.: Test Receive Characters
//
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   115,200 bps
//    WORD:   8, N, 1
//    FIFO:   No
//    INTS:   No
//
//  Date........: Mar. 1, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0751_C
#define TC0751_C

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
    int i = 0;
   unsigned char a[32]; 

main()
{
///// Declare Variables Here ///////////////////////
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART);
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 115200L, 'N', 8, 1, NO_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

// Read character transition in RBR on logic analyzer.
while(1) 
{
rreg( port->uart_base + RBR );
testDELAY(2);
}


testDELAY(10000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
