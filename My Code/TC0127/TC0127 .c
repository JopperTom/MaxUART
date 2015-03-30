//
//  File........: TC0127.c
//  Authors.....: Tom Tripp
//  Description.: Test DR bit LSR.1
//
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   57,600 bps
//    WORD:   8, E, 2
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: Feb. 22, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC1112_C
#define TC1112_C

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

    unsigned char foo, err[75], rx[75], tx[] = "ABCDEFGHIJKLMNOP";//"0123456789012345";// "Jesus is Lord!!!";
    int i;
    
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, 0x00 );
///// Your Code Here //////////////////////////////
/*
while(1)
{
  while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
  rreg(port->uart_base + RBR);
  printf(" %X \n", rreg(port->uart_base + LSR));
}
*/
   // Set Loopback Mode
   wreg( port->uart_base + MCR, MCR_LOOP_BACK );


// Send a character, wait for DR high, read RBR
  for( i = 0 ; i < 16 ; i++)
  {
 //   while( ( rreg( port->uart_base + LSR ) & LSR_THRE ) == 0x00 );
    wreg( port->uart_base + THR, tx[i] );
    while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
  }

/*   
  for( i = 0 ; i < 16 ; i++)
  {
 //   while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
//    err[i] = rreg(port->uart_base + LSR);
  }
*/  
  // Print and compare
  for( i = 0 ; i < 16 ; i++)
  {
//>>>>>>>>>>>>need to make self checking<<<<<<<<<<<<<<<<<
    putchar(rx[i]);
    putchar('=');
    putchar(tx[i]);
    putchar('\n ');
//    printf(" %X \n",err[i]);
  }






////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
