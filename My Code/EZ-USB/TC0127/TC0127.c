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
//#define POLLED

// Which interface? - define only one.
// #define IFACE_SPI 0
#define IFACE_I2C 1

#include <stdio.h>
#include <stdlib.h>
#include <ezusb.h>
#include <ezregs.h>
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */

#define XON  0x11
#define XOFF 0x13

char putchar (char c)  
{

  if (c == '\n')  
  {
    if (RI_0)  
	{
      if (SBUF0 == XOFF)  
	  {
        do  
		{
          RI_0 = 0;
          while (!RI_0);
        }
        while (SBUF0 != XON);
        RI_0 = 0; 
      }
    }
    while (!TI_0);
    TI_0 = 0;
    SBUF0 = 0x0d;                         /* output CR  */
  }
  if (RI_0)  
  {
    if (SBUF0 == XOFF)  
	{
      do  
	  {
        RI_0 = 0;
        while (!RI_0);
      }
      while (SBUF0 != XON);
      RI_0 = 0; 
    }
  }
  while (!TI_0);
  TI_0 = 0;
  return (SBUF0 = c);
}

main()
{
  SCON0 = 0x52;    /* SCON */                   /* setup serial port control */
  TMOD = 0x20;    /* TMOD */                   /* hardware ( BAUD @MHZ) */
  TCON = 0x69;    /* TCON */
  TH1 =  0xf6;    /* TH1 */

  	while(1)
  	{
		putchar ('J');
	}
}
///// Declare Variables Here ///////////////////////
/*
    BYTE foo, tx[] = "Jesus is Lord!!!";
    BYTE rx[20];
	int i;
    
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        puts( "Failed to open the port!\n" );
//        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO );
*///// Your Code Here //////////////////////////////
/*
while(1)
{
  while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
  rreg(port->uart_base + RBR);
  printf(" %X \n", rreg(port->uart_base + LSR));
}

   // Set Loopback Mode
//   wreg( port->uart_base + MCR, MCR_LOOP_BACK );


// Send a character, wait for DR high, read RBR
  for( i = 0 ; i <= 15 ; i++)
{
   //while( ( rreg( port->uart_base + LSR ) & LSR_THRE ) == 0x00 );
    //wreg( port->uart_base + THR, foo );
    while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
   // foo = rreg(port->uart_base + RBR);
//	wreg( port->uart_base + THR, foo );
    
}

   
  for( i = 0 ; i <= 16 ; i++)
  {
 //   while( ( rreg( port->uart_base + LSR ) & LSR_DR ) == 0x00 );
    rx[i] = rreg(port->uart_base + RBR);
//    err[i] = rreg(port->uart_base + LSR);
  }
  
  // Print and compare
  for( i = 0 ; i <= 16 ; i++)
  {
//>>>>>>>>>>>>need to make self checking<<<<<<<<<<<<<<<<<
    putchar(rx[i]);
//    putchar('=');
//    putchar(tx[i]);
//    putchar('\n');
//    printf(" %X \n",err[i]);
  }

*/
//while(1);


/*
///////////////////////////////////////////////////
    port_close( port );
    return( 0 );*/
//} // End of main(void)

#endif
