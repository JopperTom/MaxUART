//
//  File........: TC0154.c
//  Authors.....: Tom Tripp
//  Description.: Test RX FIFO Trigger Level 4 Bytes
//                Run test, type in data, type "Q", view printout
//                
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: April 4, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0154_C
#define TC0154_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

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
    port = port_open( COM1_UART, COM1_INTERRUPT );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_1B );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled

    // Receive Data Interrupt enabled
    wreg( port->uart_base+IER, IER_RX_DATA );

    while(1);
/*
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
*/

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
