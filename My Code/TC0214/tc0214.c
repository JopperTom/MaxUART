//
//  File........: TC0214.c
//  Authors.....: Tom Tripp
//  Description.: Test IER.3
//                This test requires some external device to toggle to CTSB
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   N/A bps
//    WORD:   N/A
//    FIFO:   NO
//    INTS:   YES
//  Date........: March 14, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0214_C
#define TC0214_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS 0x08

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

int y[10];
main()
{
///// Declare Variables Here ///////////////////////
    int x,val = 0;
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

    // Enable the IER.3
    wreg( port->uart_base + IER, IER_MODEM_STATUS );

  for(x = 0; x<10; x++ )
  {
    while ( ( y[x] = rreg(port->uart_base + MSR) & MSR_DELTA_CTS ) == 0 );
   // y[x] = rreg(port->uart_base + MSR);
  }
  for(x = 0; x<10; x++ )
  {
//    printf(" %X \n" , y[x]);
  }

    // Enable FIFO
    wreg(port->uart_base + FCR,0x01);

  for(x = 0; x<10; x++ )
  {
    while ( (rreg(port->uart_base + MSR) & MSR_DELTA_CTS ) == 0 );
    y[x] = rreg(port->uart_base + MSR);
  }
  for(x = 0; x<10; x++ )
  {
 //   printf(" %X \n" , y[x]);
  }

   
  //This test requires an external source to toggle CTSB pin.
  testDELAY(14000);
while(1);
//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
