//
//  File........: TC1020.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case to test the Manual Reset 
//  The RT99 registers are read and printed, the part is put into manual reset, the registers
//  are printed again.
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: January 25, 2004
//
// The proper interface can be found in interface.c
#ifndef TC1020_C
#define TC1020_C

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
#include "RT99reg.h"
#include "interface.c"
#include "port.c"




main()
{
///// Declare Variables Here ///////////////////////
    PORT *port;
//    char tx, rx;
int i=0;
unsigned char a = 0, b = 0, c = 0;
////////////////////////////////////////////////////
    port = port_open( COM1_UART, COM1_INTERRUPT );
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
///// Your Code Here //////////////////////////////

//    wreg( port->uart_base + IER, 0x00 ); 

//IFACE_Init();
/*
    putchar('S');
    putchar('\n');
    for(i=0; i++<=47; printf( " %X ",rreg( port->uart_base + i)) );
    putchar('\n');

    for(i=0; i++<=47; wreg( port->uart_base + i, 0xFF) );
*/
// THR
    a = rreg( port->uart_base + THR);
//    testDELAY(1000);
    wreg( port->uart_base + THR, 0xFF);
//    testDELAY(1000);
    b = rreg( port->uart_base + THR);

    // Set Manual Reset Bit
    wreg( port->uart_base + EFR, EFR_RESET ); 

    c = rreg( port->uart_base + THR);
//    testDELAY(1000);
    printf(" THR: %X, %X, %X\n",a,b,c);
// IER
    a = rreg( port->uart_base + IER);
//    testDELAY(1000);
    wreg( port->uart_base + IER, 0xFF);
//    testDELAY(1000);
    b = rreg( port->uart_base + IER);

    // Set Manual Reset Bit
    wreg( port->uart_base + EFR, EFR_RESET ); 

    c = rreg( port->uart_base + IER);
//    testDELAY(1000);
    printf(" IER: %X, %X, %X\n",a,b,c);

// MCR
    a = rreg( port->uart_base + MCR);
//    testDELAY(1000);
    wreg( port->uart_base + MCR, 0xFF);
//    testDELAY(1000);
    b = rreg( port->uart_base + MCR);

    // Set Manual Reset Bit
    wreg( port->uart_base + EFR, EFR_RESET ); 

    c = rreg( port->uart_base + MCR);
//    testDELAY(1000);
    printf(" MCR: %X, %X, %X\n",a,b,c);

// LCR
    a = rreg( port->uart_base + LCR);
//    testDELAY(1000);
    wreg( port->uart_base + LCR, 0x03);
//    testDELAY(1000);
    b = rreg( port->uart_base + LCR);

    // Set Manual Reset Bit
    wreg( port->uart_base + EFR, EFR_RESET ); 

    c = rreg( port->uart_base + LCR);
//    testDELAY(1000);
    printf(" LCR: %X, %X, %X\n",a,b,c);

//SCR
    a = rreg( port->uart_base + SCR);
//    testDELAY(1000);
    wreg( port->uart_base + SCR, 0xFF);
//    testDELAY(1000);
    b = rreg( port->uart_base + SCR);

    // Set Manual Reset Bit
    wreg( port->uart_base + EFR, EFR_RESET ); 

    c = rreg( port->uart_base + SCR);
//    testDELAY(1000);
    printf(" SCR: %X, %X, %X\n",a,b,c);

/*    for(i=0; i++<=47; printf( "%X ", rreg( port->uart_base + i)) );
    putchar('\n');
    for(i=0; i++<=63; printf( "%X ", rreg( port->uart_base + i)) );
    putchar('E');
    putchar('\n');
*/
    testDELAY(1000);
////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)

#endif
