//
//  File........: TC0170.c
//  Authors.....: Tom Tripp
//  Description.: Test MR EFR.0
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57600 bps
//    WORD:   8, N, 1
//    FIFO:   Y
//    INTS:   NO
//  Date........: April 18, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0170_C
#define TC0170_C

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
    int i, j;
    unsigned char reg[24], x;

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

    // wait here for external source to fill FIFO
    // type Q to continue to next portion of test
    while ( 1 ) 
    {
        x = getchar();
        if ( x == 'Q' )
            break;
    }

    // Set Master Reset
    wreg( port->uart_base + EFR, EFR_RESET );

    
    for(j=0; j<24; j++ )
    reg[j]= rreg( com->uart_base + j);

/*
    // FIFO is read one character at a time and
    // stored in an array
    for( i = 0; i < 16; i++)
    {
        lsr[i] = rreg( port->uart_base + LSR );
        rx[i] = rreg( port->uart_base + RBR );
    }
*/    
    // the array is output to be visually checked
    for( i = 0; i < 24; i++)
    {
          printf(" %X ",reg[i]);
    }


    // delay in order to complete output
    testDELAY(10000);
    puts("\n End of Test\n");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
