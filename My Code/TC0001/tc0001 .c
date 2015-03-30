//
//  File........: TC0001.c
//  Authors.....: Bob Kelly/Tom Tripp
//  Description.: Print out the value of the RT99 RCR register.
//  
//  Conditions:
//    IFACE:  SPI
//    BAUD:   N/A bps
//    WORD:   N/A
//    FIFO:   No
//    INTS:   No
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0001_C
#define TC0001_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
//#define POLLED

// Which interface? - define only one.
#define IFACE_SPI
//#define IFACE_I2C

//#include <stdio.h>
//#include <stdlib.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation

BYTE x;

main()
{
// This test is done with the RT99 un-initialized.
///// Declare Local Variables Here /////////////////
////////////////////////////////////////////////////
/*    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if memory was allocated
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
*//// Your Code Here //////////////////////////////

  POR();
  IFACE_Init();

// These reads can be viewed on the bus.
rreg(RCR);
rreg(RCR);
rreg(RCR);
rreg(RCR);
rreg(RCR);
rreg(RCR);
rreg(RCR);

 x = rreg(RCR);
 printf("RCR: %X\n", x );

testDELAY(14000);

////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)


#endif
