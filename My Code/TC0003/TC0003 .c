//
//  File........: TC0003.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that writes to all registers.
//  some registers are read only. This test has limited usefulness.
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   No
//    INTS:   Yes or No
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0003_C
#define TC0003_C

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

int i = 0;
unsigned char a[12];

main()
{
///// Declare Variables Here ///////////////////////


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


for( i = 0 ; i <= 12 ; i++)
{
  wreg( i, 0xFF );
}

for( i = 0 ; i <= 12 ; i++)
{
  a[i] = rreg( i );
}

for( i = 0 ; i <= 12 ; i++)
{
  putchar(a[i]);
}



////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
