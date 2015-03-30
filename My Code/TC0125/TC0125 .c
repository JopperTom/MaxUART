//
//  File........: TC0125.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case to test the TEMT (bit6) of the LSR. With and without FIFO and INTERRUPTS enabled.
//                The TEMT should be set to zero whenever data is in the TSR and/or THR and is being xmitted. It should 
//                contain to 1 when the data has all been transmitted and both the TSR and THR are empty.
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   s8,400bps
//    WORD:   8, N, 1
//    FIFO:   VARIABLE
//    INTS:   NO
//  Date........: Feb 9, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0125_C
#define TC0125_C

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

    unsigned char tx_data = 0x54, rx_data = 0x00;

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

    // bit6 should set at default
    if( (rreg( LSR) & LSR_TEMT) == 0)
      puts(" ERR: TEMT default wrong.\n"); 

    // Set Loopback mode
    wreg( port->uart_base + MCR, MCR_LOOP_BACK ); 


    // Disable FIFO
    wreg(port->uart_base + FCR,0x01);
    // Send data
    wreg(port->uart_base + THR,tx_data); // send a Byte
    while( (rreg(port->uart_base + LSR) & LSR_TEMT) == 0);
    // Receive data
    rx_data = rreg( port->uart_base + RBR); 
    // Check data
    if(rx_data != tx_data)
      puts(" ERR: TEMT wrong. w/o FIFO\n"); 
    putchar(rx_data);    
    putchar('\n');    
    
    // Enable FIFO
    wreg(port->uart_base + FCR,0x01);
    // Send data
    wreg(port->uart_base + THR,tx_data); // send a Byte
    while( (rreg(port->uart_base + LSR) & LSR_TEMT) == 0);
    // Receive data
    rx_data = rreg( port->uart_base + RBR); 
    // Check data
    if(rx_data != tx_data)
      puts(" ERR: TEMT wrong. w/FIFO\n"); 
    putchar(rx_data);    
    putchar('\n');    

    puts( " End of Test!\n");

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
