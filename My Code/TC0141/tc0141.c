//
//  File........: TC0141.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that uses the SPI interface to toggle the value in
//  the RT99 MCR.1 register bit and see if the RTSB (/RTS) pin toggles. It is required to
//  have a logic analyzer connect to the RTSB pin or some other way of detecting the change.

//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   NO
//    INTS:   NO
//  Date........: February 24, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0141_C
#define TC0141_C

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
    int x;
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

//for(x = 0; x<=100; x++)
//    wreg(MCR, 0x00);// RTSB High
for(x = 0; x<=500; x++);
    wreg(MCR, 0x02);// RTSB Low

for(x = 0; x<=500; x++);
    wreg(MCR, 0x00);// RTSB High

for(x = 0; x<=1000; x++);
    wreg(MCR, 0x02);// RTSB Low

for(x = 0; x<=10; x++);
    wreg(MCR, 0x00);// RTSB High
for(x = 0; x<=50; x++);
    wreg(MCR, 0x02);// RTSB Low
for(x = 0; x<=10; x++);
    wreg(MCR, 0x00);// RTSB High





/*
for(x = 0; x<=1000; x++)
  {
    val ^= 0x02; // Toggles MCR.1
    wreg(MCR, val);
  }
for(x = 0; x<=100; x++)
    wreg(MCR, 0x00);
for(x = 0; x<=100; x++)
    wreg(MCR, 0x02);
  */
  //This test requires an external source to monitor the RTSB pin.

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
