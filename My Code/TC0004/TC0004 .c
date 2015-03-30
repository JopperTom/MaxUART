//
//  File........: TC0004.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that reads all RT99 register default values.
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

#ifndef TC0004_C
#define TC0004_C

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

int i[24];
int j = 0;

main()
{
///// Declare Local Variables Here /////////////////

// RT99 unconfigured //////
////////////////////////////////////////////////////
/*    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
///// Your Code Here //////////////////////////////
*/
POR();
IFACE_Init();


for(j=0; j<=24; j++ )
i[j]= rreg( com->uart_base + j);

//for(j=0; j<=12; printf("%X \n",i[j++]) );

//for(j=0; j<12; printf("%X",j++));



////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)

#endif
