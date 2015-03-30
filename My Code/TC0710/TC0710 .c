//
//  File........: TC07110.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that uses the SPI interface to receive a string of characters at 38,400, 
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

#ifndef TC0710_C
#define TC0710_C

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
 // char string[] = "Maxim - The quick brown fox came down with the pox.";
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
///// Your Code Here //////////////////////////////

while(1)
{

   i = rreg( port->uart_base + RBR);
  putchar(i);
  //printf( " %c\n ", i ) );
}


////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
