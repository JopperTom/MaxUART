//
//  File........: TC0160.c
//  Authors.....: Tom Tripp
//  Description.: Test EFR.4 CLK Out
//                
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38400 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   YES
//  Date........: April 7, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0160_C
#define TC0160_C

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

    wreg(port->uart_base + EFR, EFR_CLK_OUT );

    // Monitor RTSB for a CLK signal    
    while(1);
    
//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
