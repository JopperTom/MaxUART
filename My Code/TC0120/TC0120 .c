//
//  File........: TC0120.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case to test the THRE (bit5) of the LSR. With and without FIFO and INTERRUPTS enabled.
//                The THRE should be set to zero whenever data is written to the THR and should return to 1 when the data
//                is shifted out of the THR and the THR is ready for more data.
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   s8,400bps
//    WORD:   8, N, 1
//    FIFO:   VARIABLE
//    INTS:   NO
//  Date........: Feb 9, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0120_C
#define TC0120_C

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
unsigned char a[4];
int x = 0;

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

    // Disable FIFO
    wreg(port->uart_base + FCR,0x00);
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
    wreg(port->uart_base + FCR,0x01);
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
puts("End of Test");
testDELAY(14000);
////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
