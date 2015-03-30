//
//  File........: TC0050.c
//  Authors.....: Tom Tripp
//  Description.: Test the NO_NAME register
//
//  NOTE:
//  Currently, it is simpler to have Bob Kelly verify proper
//  operation of this register.
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38400 bps
//    WORD:   8N1
//    FIFO:   Yes
//    INTS:   No
//  Date........: April 5, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0050_C
#define TC0050_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
#define POLLED

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

BYTE x[] = "Maxim! - The Quick Brown Fox Came Down With The Pox.";
WORD y;
int i;

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
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 );
//// Your Code Here //////////////////////////////

  y = rreg(port->uart_base + LSR);

 do
 {
    for ( i = 0 ; (x[i] != 0x00) || (y != 'Q') ; i++)
    {
       // putchar(((BYTE)(y>>8))& 0x60);
      if((((BYTE)(y >> 8)) & LSR_THRE) == 0 )
        y = duplex(x[i]);
      if( (BYTE)(y >> 8) & LSR_DR )
       ;// putchar((BYTE)y);
    }
 } while((BYTE)y != 'Q');



testDELAY(14000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)


#endif
