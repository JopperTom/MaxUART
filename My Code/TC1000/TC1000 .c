//
//  File........: TC1000.c
//  Authors.....: Tom Tripp
//  Description.: Test the Software Sleep Mode 
//  The RT99 registers are read and printed, the part is put into SW Sleep mode, the registers
//  are printed again, the part is awaken using the HCR register and the register values are 
//  printed again.
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: January 20, 2004
//
// The proper interface can be found in interface.c
#ifndef TC1000_C
#define TC1000_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

// Which interface? - define only one.
#define IFACE_SPI
//#define IFACE_I2C

#include <stdio.h>
#include <stdlib.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"
#include "interface.c"
#include "port.c"

main()
{
    PORT *port;
//    char tx, rx;
int i=0, x;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
__disable_interrupt();
 wreg( com->uart_base + IER, 0x00 ); 

// printf( "\nStart\n" );

for(i=0; i++<=7; printf( "%X ", rreg( com->uart_base + i)) );

// printf( "\npre-Sleep1\n" );

 wreg( com->uart_base + HCR, HCR_SWSHDN ); 

 printf( "\npost-Sleep2\n" );

for(i=0; i++<=7; printf( "%X ", rreg( com->uart_base + i)) );

// printf( "\npre-wake\n" );

 wreg( com->uart_base + HCR, HCR_WAKE ); 

// printf( "\npost-wake\n" );
  
for(i=0; i++<=7; printf( "%X ", rreg( com->uart_base + i)) );

// printf( "\nagain?\n" );
 
for(i=0; i++<=7; printf( "%X ", rreg( com->uart_base + i)) );

 printf( "\nEnd\n" );

    port_close( port );
    return( 0 );
}
#endif
