//
//  File........: TC0212.c
//  Authors.....: Tom Tripp
//  Description.: Tests IER.1  
//  8N1 with FIFOs disabled
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    38400 bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes
//
//  Date........: March 14, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0212_C
#define TC0212_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
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
  int i = 0;
  char string[] = "Maxim - The quick brown fox came down with the pox.";

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, 0x00 ); 
///// Your Code Here //////////////////////////////

    // Enable the IER.1
    wreg( port->uart_base + IER, 0x02 );

  for( i = 0 ; string[i] != 0 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
    wreg( THR, (int)string[i] );
  }

testDELAY(14000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
