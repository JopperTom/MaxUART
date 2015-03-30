//
//  File........: TC0146.c
//  Authors.....: Tom Tripp
//  Description.: Test SCR Register
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38400 bps
//    WORD:   8, N, 1
//    FIFO:   Y+N
//    INTS:   NO
//  Date........: February 24, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0146_C
#define TC0146_C

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
    unsigned char x = 0;
    int i = 0;
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
    // Write all values to SCR and read them back and compare.
    for( i = 0; i <= 255; i++)
    {
      wreg(port->uart_base + SCR, i);
          
      x = rreg(port->uart_base + SCR);
      if ( (int)x != i ) puts("Error");
    }

    // Disable FIFO    
    // Write all values to SCR and read them back and compare.
    wreg(port->uart_base + FCR, 0x00);

    for( i = 0; i <= 255; i++)
    {
      wreg(port->uart_base + SCR, i);
          
      x = rreg(port->uart_base + SCR);
      if ( (int)x != i ) puts("Error");
    }
    puts(" End of Test.");   

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
