//
//  File........: TC0159.c
//  Authors.....: Tom Tripp
//  Description.: Test RX FIFOs ENABLE FCR.0 - Does clearing this bit clear the FIFO
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38400 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: April 17, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0159_C
#define TC0159_C

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
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_1B );


/////// Your Code Here //////////////////////////////


/////STEP 1////////////////////////////////////////////
// Does clearing FIFO enable disable FIFO?
    // FIFO are enabled

    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( getchar() != 'Q' );


    // Clear FIFO Enable
    wreg( port->uart_base + FCR, 0x00 );

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
//          printf(" %X \n ", lsr[i]);
    }
          putchar('\n');


/////STEP 2////////////////////////////////////////////
// Does clearing FIFO enable clear the FIFO?


    // Set FIFO Enable
    wreg( port->uart_base + FCR, 0x01 );


    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( getchar() != 'Q' );


    // Clear FIFO Enable
    wreg( port->uart_base + FCR, 0x00 );
    // Set FIFO Enable
    wreg( port->uart_base + FCR, 0x01 );


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
//          printf(" %X \n ", lsr[i]);
    }
          putchar('\n');

/////STEP 3 - Does the FIFO still work? ///////////////////
    // Set FIFO Enable
    wreg( port->uart_base + FCR, 0x01);
    
    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( getchar() != 'Q' );


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
//          printf(" %X \n ", lsr[i]);
    }
          putchar('\n');


    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test\n");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
