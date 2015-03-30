//
//  File........: TC0143.c
//  Authors.....: Tom Tripp
//  Description.: Test MCR.4 Loop Back - this test should show that the CTS is disconnected in LoopBack mode
//                This test requires some external device to toggle to CTSB
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   Y + N
//    INTS:   NO
//  Date........: February 23, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0143_C
#define TC0143_C

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


  wreg(port->uart_base + MCR, MCR_LOOP_BACK);


for(x = 0; x<=10; x++ )
  {

    while ( (rreg(port->uart_base + MSR) & MSR_CTS ) == 0x10 );
    printf(" %X \n" , rreg(port->uart_base + MSR));
  }


  
  //This test requires an external source to monitor the RTSB pin.

//////////////////////////////////////////////////////////  
//    port_close( port );
    return( 0 );
}

#endif
