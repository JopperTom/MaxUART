//
//  File........: TC0900.c
//  Authors.....: Tom Tripp
//  Description.: Test AUTO CTS
//                This test requires a signal generator or some external
//                equipment to pull the CTSB pin low.
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 22, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0900_C
#define TC0900_C

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
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, FCR_RX_TRIG_1B );
///// Your Code Here //////////////////////////////
   // Enable Interrupts
   wreg( port->uart_base + IER, 0x02 );

  // Enable AutoCTS
  wreg( port->uart_base + EFR, EFR_AUTO_CTS );
while(1){
  // Write data to THR one char at a time
  for( i = 0 ; i <= 255 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_THRE ) == 0 );
    wreg(  port->uart_base + THR, (int)i );
    
  }
    while( ( rreg( port->uart_base + LSR ) & LSR_THRE ) == 0 );
}

// Requires external source to toggle CTSB
// When CTSB is held high data should not transmit

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
