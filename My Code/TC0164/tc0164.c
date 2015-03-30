//
//  File........: TC0164.c
//  Authors.....: Tom Tripp
//  Description.: Test RX FIFOs trigger 14Byte - drop below level
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: May 31, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0164_C
#define TC0164_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS IER_RX_DATA

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
    unsigned char rx_data[16];

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_14B );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled
    
    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( getchar() != 'Q' );

    // Run test
    // Load RX FIFO
    // Set LA to monitor when IRQB line goes high.
    // Type Q to activate test
    // Examine waves to see when IRQB is cleared
    // results should be as FIFO goes from 14 characters
    // to 13 characters.

    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test\n");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
