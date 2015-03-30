//
//  File........: TC0700.c
//  Authors.....: Tom Tripp
//  Description.: xmit a string of characters at 38,400, 
//  8N1 with FIFOs disabled
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: December 10, 2004
//
//  The proper interface can be found in interface.c

#ifndef TC0700_C
#define TC0700_C

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
 // char string[] = "!!!###$$$%%%&&&''((()))***+++,,,---???<<<>>>";
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 ); 
///// Your Code Here //////////////////////////////

  for( i = 0 ; string[i] != 0 ; i++)
  {
    while( ( rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0x00 );
    wreg( THR, (int)string[i] );
  }


////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
