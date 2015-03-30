//
//  File........: TC0000.c
//  Authors.....: Bob Kelly/Tom Tripp
//  Description.: This is a test case that toggles the MCR.1 to see if the RTSB (/RTS) pin toggles. 
//  It is required to externally monitor the RTSB pin.

//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38400 bps
//    WORD:   8N1
//    FIFO:   NO
//    INTS:   NO
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0000_C
#define TC0000_C

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


//// Your Code Here //////////////////////////////

  wreg(MCR, 0x02); //RTSB Low
  testDELAY(500);
  wreg(MCR, 0x00); //RTSB High
  testDELAY(1000);
  wreg(MCR, 0x02); //RTSB Low
  testDELAY(500);

for(x = 0; x<=100000; x++)
  {
    val ^= 0x02; // Toggles val
    wreg(MCR, val);
  }
  
  //This test requires an external source to monitor the RTSB pin.

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
