//
//  File........: TC0145.c
//  Authors.....: Tom Tripp
//  Description.: Test CTS MSR.4
//                This test requires some external device to toggle to CTSB
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   N/A bps
//    WORD:   N/A
//    FIFO:   NO
//    INTS:   NO
//  Date........: February 23, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0145_C
#define TC0145_C

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
    BYTE val[11];

    int x;

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
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, 0x00 );


/////// Your Code Here //////////////////////////////


  for(x = 0; x<10; x++ )
  {
    while ( (val[x] = rreg(port->uart_base + MSR) & MSR_DELTA_CTS ) == 0 );
     //rreg(port->uart_base + MSR);
  }

  for(x = 0; x<10; x++ )
  {
    putchar(' ');
    putchar(val[x]);
    putchar('\n');
  }

    // Enable FIFO
    wreg(port->uart_base + FCR,0x01);
    
  for(x = 0; x<10; x++ )
  {

    while ( (val[x] = rreg(port->uart_base + MSR) & MSR_DELTA_CTS ) == 0 );
   // rreg(port->uart_base + MSR);
  }

  for(x = 0; x<10; x++ )
  {
    putchar(' ');
    putchar(val[x]);
    putchar('\n');
  }

testDELAY(14000);   
  //This test requires an external source to monitor the CTSB pin.

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
