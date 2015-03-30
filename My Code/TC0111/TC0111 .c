//
//  File........: TC0111.c
//  Authors.....: Tom Tripp
//  Description.: This test case tests to make sure that no interrupts
//                are generated when all interrupts are disabled.
//                NOTE: an external device will need to monitor IRQB
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   s8,400bps
//    WORD:   8, N, 1
//    FIFO:   VARIABLE
//    INTS:   NO
//  Date........: Feb 28, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0111_C
#define TC0111_C

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

unsigned char a[4];
int x = 0;
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO ); 
///// Your Code Here //////////////////////////////

    // Disable FIFO
    wreg(port->uart_base + FCR,0x00); // M
    // Send data
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0xAA); // toggle
a[0] = rreg( port->uart_base + LSR); // bit5 should clear
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0xAA); // toggle
a[1] = rreg( port->uart_base + LSR); // bit5 should clear

    // Enable FIFO
    wreg(port->uart_base + FCR,0x01); // M
    // Send data
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0xAA); // toggle
a[2] = rreg( port->uart_base + LSR); // bit5 should clear
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0x4D); // M
    wreg(port->uart_base + THR,0xAA); // toggle
a[3] = rreg( LSR); // bit5 should clear

for(x=0; x <= 3; x++)
{
  printf(" %X \n", a[x]);
  if( a[x] != 0 ) 
    puts("Error: THRE not updated properly.");
}

////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)

#endif
