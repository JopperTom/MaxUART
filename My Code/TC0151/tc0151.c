//
//  File........: TC0151.c
//  Authors.....: Tom Tripp
//  Description.: Test RX FIFOs RESET FCR.1
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: February 28, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0151_C
#define TC0151_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
#define POLLED

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
    int i;
    unsigned char tx_data[16] = "JESUS LIVES NOW!";
    unsigned char rx_data[16];
    unsigned char lsr[16];

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 57600L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_1B );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled
    
    // Loopback Mode
    wreg(port->uart_base + MCR, MCR_LOOP_BACK);

    // Send data
    for( i=0; i<16; i++)
    {
      wreg( port->uart_base + THR, tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_THRE) != 0x20);
    }

    // Set RX FIFO Reset
    wreg( port->uart_base + FCR, 0x03 );

    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
        lsr[i] = rreg( port->uart_base + LSR );
        rx_data[i] = rreg( port->uart_base + RBR );
    }

    // the array is output to be visually checked
    for( i = 0; i < 16; i++)
    {
      if((rx_data[i] != 0x00) || (lsr[i] != 0x60))
      {
        puts(" ERROR in FIFO\n"); 
        break;
      }
    }

/////// Does the FIFO still function? /////////////////////////

    // Send data
    for( i=0; i<16; i++)
    {
      wreg( port->uart_base + THR, tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_THRE) != 0x20);
    }

    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
        lsr[i] = rreg( port->uart_base + LSR );
        rx_data[i] = rreg( port->uart_base + RBR );
    }

    // the array is output to be visually checked
    for( i = 0; i < 16; i++)
    {
          putchar(' ');
          putchar(rx_data[i]);
          putchar(' ');
      //    putchar(lsr[i]);
//          printf(" %X \n ", lsr[i]);
    }
          putchar('\n');
/*
    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
//    while ( getchar() != 'Q' );

    // Set RX FIFO Reset
//    wreg( port->uart_base + FCR, 0x03 );

    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
        lsr[i] = rreg( port->uart_base + LSR );
        rx_data[i] = rreg( port->uart_base + RBR );
    }
    
    // the array is output to be visually checked
    for( i = 0; i < 16; i++)
    {
          putchar(' ');
          putchar(rx_data[i]);
          putchar(' ');
          putchar(lsr[i]);
       //   printf(" %X \n ", lsr[i]);
    }
          putchar('\n');

*/
    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test.\n");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
