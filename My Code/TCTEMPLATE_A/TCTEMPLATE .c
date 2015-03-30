//
//  File........: TCxxxx.c
//  Authors.....: Your Name Here
//  Description.: 
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: Today
//
//  The proper interface can be found in interface.c

#ifndef TCXXXX_C
#define TCXXXX_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS IER_LINE_STATUS | IER_MODEM_STATUS | IER_RX_DATA | IER_THRE

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
///// Declare Global Variables Here ////////////////

////////////////////////////////////////////////////
main()
{
///// Declare Local Variables Here /////////////////

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if port is valid
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, FCR_RX_TRIG_1B );
///// Your Code Here //////////////////////////////



    testDELAY(14000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
