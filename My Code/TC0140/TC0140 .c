//
//  File........: TC0140.c
//  Authors.....: Tom Tripp
//  Description.: Test SET BREAK LCR bit6
//                When this test is run the LA should see the TX
//                pin go low.
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   8, N, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 22, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0140_C
#define TC0140_C

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
    unsigned char i;

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'N', 8, 1, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

    wreg( port->uart_base + LCR, 0x43 );
    testDELAY(5000);
    wreg( port->uart_base + LCR, 0x03 );
    testDELAY(5000);
    wreg( port->uart_base + LCR, 0x43 );
    testDELAY(5000);
    wreg( port->uart_base + LCR, 0x03 );
    testDELAY(5000);



//    wreg( port->uart_base + LCR, i & 0xBF );
//    testDELAY(15000);





////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
