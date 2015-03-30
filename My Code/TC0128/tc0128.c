//
//  File........: TC0128.c
//  Authors.....: Tom Tripp
//  Description.: Test LSR.4 BreakInt - this is a blind read of the LSR
//                This test requires some external device to pull down RX_INT
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   Y + N
//    INTS:   NO
//  Date........: February 23, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0128_C
#define TC0128_C

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

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 115200L, 'N', 8, 1, NO_FIFO, 0x00 );


/////// Your Code Here //////////////////////////////

for(;;)
  {
    rreg(port->uart_base + LSR);
    testDELAY(20);
  }
  
  // This test requires an external source to pull down the RX_PAR pin.

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
