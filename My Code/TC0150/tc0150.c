//
//  File........: TC0150.c
//  Authors.....: Tom Tripp
//  Description.: Test FIFOs ENABLED IIR.5 and IIR.7
//                This test case is self checking. 
//                Simply run it and check results.
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   38400 bps
//    WORD:   8, N, 1
//    FIFO:   Y+N
//    INTS:   NO
//  Date........: February 24, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0150_C
#define TC0150_C

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
    unsigned char x = 0, y = 0;


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
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 );


/////// Your Code Here //////////////////////////////

    // FIFO are enabled

    // Check operation of IIR.6 and IIR.7
    x=rreg(port->uart_base + IIR);

    // Disable FIFO    
    wreg(port->uart_base + FCR, 0x00);
    
    // Check operation of IIR.6 and IIR.7
    y=rreg(port->uart_base + IIR);


    printf(" %X \n" ,x );
    if ( ( x & 0xF0) != 0xC0) puts("Error");

    printf(" %X \n" ,y );
    if ( ( y & 0xF0) != 0x00) puts("Error");
    
    testDELAY(14000);
    puts("End of Test.");
    

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
