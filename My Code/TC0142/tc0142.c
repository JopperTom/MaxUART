//
//  File........: TC0145.c
//  Authors.....: Tom Tripp
//  Description.: Test Loop Back MCR.4
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57,600 bps
//    WORD:   8, N, 1
//    FIFO:   N + Y
//    INTS:   NO
//  Date........: February 25, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0142_C
#define TC0142_C

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
    int a = 0,b = 0,c = 0,d = 0,e = 0,f = 0;


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
    port_set( port, 57600L, 'N', 8, 1, NO_FIFO, 0x00 );

/////// Your Code Here //////////////////////////////

  wreg(port->uart_base + MCR, MCR_LOOP_BACK);

    wreg(port->uart_base + MCR, 0x12 );
    a = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    b = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x12 );
    c = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    d = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x12 );
    e = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    f = rreg(port->uart_base + MSR);
  
    if( e==f || c==d )
    { 
      puts("Error 1");
    }
  //  printf(" a:%X b:%X c:%X d:%X e:%X f:%X \n", a, b, c, d, e, f); 

    // Enable FIFO
    wreg(port->uart_base + FCR,0x01);
    wreg(port->uart_base + MCR, 0x12 );
    a = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    b = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x12 );
    c = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    d = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x12 );
    e = rreg(port->uart_base + MSR);
    wreg(port->uart_base + MCR, 0x10 );
    f = rreg(port->uart_base + MSR);
  
    if( e==f || c==d )
    { 
      puts("Error 2");
    }
 //while(1);

  testDELAY(14000);
  puts("End of Test.");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
