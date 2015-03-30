//
//  File........: TC0100.c
//  Authors.....: Tom Tripp
//  Description.: Read all register default values prior to setting any other functionality.
//  
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   No
//    INTS:   No
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0100_C
#define TC0100_C

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

int i[24];

main()
{
///// Declare Variables Here ///////////////////////

int j = 0;

/*///////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, 0x00 );
*///// Your Code Here //////////////////////////////

// Reset chip - toggle UVLOB
POR();
// Start up SPI
IFACE_Init();

// Read Registers
for(j=0; j<=24; j++ )
i[j]= rreg( com->uart_base + j);

// Print values
for(j=0; j<=24; printf("%x: %X \n",j , i[j++]) );


////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)

#endif
