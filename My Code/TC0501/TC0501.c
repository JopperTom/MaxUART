//
//  File........: TC0501.c
//  Authors.....: Tom Tripp
//  Description.: Test IrDa 6X Mode
//
//
//  Conditions:
//    IFACE:  SPI
//    BAUD:   38,400 bps
//    WORD:   8, N, 1
//    FIFO:   Yes
//    INTS:   No
//
//  Date........: June 1, 2005
//
//  The proper interface can be found in interface.c

#ifndef TC0501_C
#define TC0501_C

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
   unsigned char rx[75];
main()
{
///// Declare Variables Here ///////////////////////

    unsigned char foo, err[75], tx[] = "MaximMaximMaximMaximMaximMaximMaximMaximMaximMaximMaximMaximMaximMaximMaxim";
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
    port_set( port, 115200L, 'N', 8, 1, NO_FIFO, 0x00 );
///// Your Code Here //////////////////////////////

   // Set IrDa 6X Mode
   wreg( port->uart_base + EFR, EFR_IRDA_6X );
//while(1){
// Send a character, wait for DR high, read RBR
  for( i = 0 ; i < 75 ; i++)
  {
    wreg( port->uart_base + THR, tx[i] );
 //   while( ( rreg( port->uart_base + LSR ) & LSR_THRE ) == 0x00 );
    while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
  }

  // Print and compare
  for( i = 0 ; i < 75 ; i++)
  {
    putchar(rx[i]);
    putchar('=');
    putchar(tx[i]);
//    putchar('\n');
    putchar('\n');
//    printf(" %X \n",err[i]);
  }
//}
  putchar('\n ');
  puts("End Of Test");
  testDELAY(10000);



////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
