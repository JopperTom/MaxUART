//
//  File........: TC0910.c
//  Authors.....: Tom Tripp
//  Description.: Test AUTO RTS
//                An external source is needed to load the RX FIFO
//                and to monitor the RTSB pin.
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Mar. 8, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0910_C
#define TC0910_C

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

//    char tx;
    int i;
    unsigned char j;

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if port was initialized
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_14B );
///// Your Code Here //////////////////////////////

    // FIFO are enabled

    // Enable AutoRTS
    wreg( port->uart_base + EFR, EFR_AUTO_RTS );

    
    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( getchar() != 'Q' );


    // Run test
    // Load RX FIFO
    // Set LA to monitor when IRQB line goes high.
    // Type Q to activate test
    // Examine waves to see when IRQB is cleared
    // results should be as FIFO goes from 4 characters
    // to three characters.


    // delay in order to complete output
    testDELAY(15000);
    puts(" End of Test\n");

/*  for( i = 0 ; i <= 255 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 1 );
    wreg(  port->uart_base + THR, (int)i );
    
  }
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 1 );
*/
// Requires external source to load FIFO and monitor RTSB
/*
while(getchar() != 'x')
{
    rreg( port->uart_base + RBR );
}
*/
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
