//
//  File........: TC0200.c
//  Authors.....: Tom Tripp
//  Description.: xmit a string of characters at 38,400, 8N1 with FIFOs disabled
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0200_C
#define TC0200_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS 0x00

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

// 5D transmitted will look like BA on Logic analyzer
// 10111010
// EVEN (parity bit) = HIGH and ODD (parity bit) = LOW

// 55 transmitted will look like AA on Logic analyzer
// 10101010 
// EVEN (parity bit) = LOW and ODD (parity bit) = HIGH

// <<<remember the parity bit is included in calculating parity>>>


main()
{
///// Declare Variables Here ///////////////////////
    unsigned char tx[] = {0x5D, 0x55, 0x4D, 0x58};
//    unsigned char tx[] = "Maxim - The Quick Brown Fox Came Down With The Pox. ";
    int i;
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
//    port_set( port, 38400L, 'N', 8, 1, NO_FIFO, 0x00 );
//    port_set( port, 38400L, 'O', 8, 1, NO_FIFO, 0x00 );
//    port_set( port, 38400L, 'E', 8, 1, NO_FIFO, 0x00 );
//    port_set( port, 38400L, 'M', 8, 1, NO_FIFO, 0x00 );
    port_set( port, 38400L, 'S', 8, 1, NO_FIFO, 0x00 );
///// Your Code Here //////////////////////////////



//    while ( 1 ) 
//    {
        for ( i = 0; i < 4 ; i++)
        {
            while( port_putc( tx[i], port ) != 0 )
              testDELAY(70000);

        }

//        if ( tx == 'Q' )
//            break;
/*        else
            rx = port_getc( port );
        if ( rx == tx )
            putchar( 'Y' );
        else
            putchar( 'N' );
*/ // }
              testDELAY(150000);

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
