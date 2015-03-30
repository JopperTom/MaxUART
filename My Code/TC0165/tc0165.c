//
//  File........: TC0165.c
//  Authors.....: Tom Tripp
//  Description.: Test TX FIFOs RESET FCR.2
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: March 3, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0165_C
#define TC0165_C
    int i;
    unsigned char rx[16] = 0;
    unsigned char tx[] = "Maxim - The Quick Brown Fox came down with the pox.";
    unsigned char lsr[16];

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
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled
    
    // FIFO is written one character at a time and
    for( i = 0; i < 16; i++)
    {
        wreg( port->uart_base + THR, tx[i] );
    }


    testDELAY(2500);

    // Set TX FIFO Reset
    wreg( port->uart_base + FCR, 0x05 );

  // Monitor xmit data with LA. Make sure last character is not truncated.


    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test\n");
//while(1);
//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
