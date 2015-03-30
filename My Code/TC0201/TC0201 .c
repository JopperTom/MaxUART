//
//  File........: TC0201.c
//  Authors.....: Tom Tripp
//  Description.: This is a test of Full Duplex operation using interrupts and AutoRTS/AutoCTS
//  Asynch data received on the RX pin should be re-transmitted on the TX pin.
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

#ifndef TC0201_C
#define TC0201_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS (IER_RX_DATA /* | IER_LINE_STATUS*/ )

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

int i;

main()
{
///// Declare Variables Here ///////////////////////

//    unsigned char tx[] = "Maxim - The Quick Brown Fox Came Down With The Pox. ";
    
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if memory was allocated
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_1B );
///// Your Code Here //////////////////////////////

    // Enable AutoRTS and AutiCTS
    wreg( port->uart_base + EFR, (EFR_AUTO_CTS | EFR_AUTO_RTS) );


    while ( 1 ) 
    {
            
            // Wait for incoming transmission
            while( ((i = port_getc(port)) < 0)  );
             
            // Transmit received data back to sendor
            while( (port_putc( i, port ) != 0)  );
//              testDELAY(200);

    }

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
