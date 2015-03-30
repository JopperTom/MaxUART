//
//  File........: TC0153.c
//  Authors.....: Tom Tripp
//  Description.: Test TX FIFOs RESET FCR.2
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: March 3, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0153_C
#define TC0153_C
    int i;
    unsigned char rx[16] = 0;
    unsigned char tx[] = "Maxim - The Quick Brown Fox came down with the pox.";
    unsigned char lsr[16];

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

////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 1200L, 'N', 8, 1, YES_FIFO, 0x00 );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled
    
    // Enable LOOP BACK
    wreg(port->uart_base + MCR, MCR_LOOP_BACK);

    // FIFO is written to one character at a time and
    for( i = 0; i < 16; i++)
    {
        wreg( port->uart_base + THR, tx[i] );
    }

    // Set TX FIFO Reset
    wreg( port->uart_base + FCR, 0x05 );
//    printf("\nfr: %X \n ", (rreg( port->uart_base + FCR) & 0x04));

    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
       // while( ( (lsr[i] = rreg( port->uart_base + LSR )) & LSR_DR ) == 0x00 );//rreg(port->uart_base + RBR);
        lsr[i] = rreg( port->uart_base + LSR );
        rx[i] = rreg( port->uart_base + RBR );
    }
    
    // the array is output to be visually checked
    for( i = 0; i < 16; i++)
    {
          putchar(rx[i]);
          putchar(' ');
          putchar(lsr[i]);
          putchar('\n');
//          printf(" %X %X \n ", rx[i], lsr[i]);
    }


    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test\n");
//while(1);
//////////////////////////////////////////////////////////  
//    port_close( port );
    return( 0 );
}

#endif
