//
//  File........: TC1114.c
//  Authors.....: Tom Tripp
//  Description.: Test Parity Error - MARK. bit2 of the LSR.
//                The Parity of this test will be set to MARK and non-MARK parity data
//                will be sent from test system to see if the LSR bit2 registers the
//                error.
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   8, M, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 22, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC1114_C
#define TC1114_C

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
    int i;
    unsigned char rx[16];
    unsigned char lsr[16];

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'M', 8, 1, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

    // FIFO are enabled

    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( 1 ) 
    {
        rx[0] = getchar();
        if ( rx[0] == 'Q' )
            break;
    }

    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
        lsr[i] = rreg( port->uart_base + LSR );
        rx[i] = rreg( port->uart_base + RBR );
    }
    
    // the array is output to be visually checked
    for( i = 0; i < 16; i++)
    {
          putchar(rx[i]);
          putchar(' ');
          printf(" %X \n ", lsr[i]);
    }

    // delay in order to complete output
    testDELAY(10000);


////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
