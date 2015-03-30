//
//  File........: TC0911.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case 
//  
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: June 13, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0911_C
#define TC0911_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

#define ACTIVE_INTERRUPTS (IER_RX_DATA /* | IER_LINE_STATUS*/ )

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

int i = 0, last_i = 0, oop = 0, count = 0;

main()
{
///// Declare Variables Here ///////////////////////

//    unsigned char tx[] = "Maxim - The Quick Brown Fox Came Down With The Pox. ";
    
////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, YES_FIFO, FCR_RX_TRIG_14B );
///// Your Code Here //////////////////////////////
    last_i = 'm';

    // Enable AutoRTS and AutiCTS
    wreg( port->uart_base + EFR, EFR_AUTO_RTS );

    while ( 1 ) 
    {
            
            while( ((i = port_getc(port)) < 0) /*&& (err == 0)*/ ); 


            switch(i)
            {
            
              case 'M' :
              if(last_i != 'm') {oop = i; exit(1);}
              break;
              
              case 'a' :
              if(last_i != 'M') {oop = i; exit(1);}
              break;
              
              case 'x' :
              if(last_i != 'a') {oop = i; exit(1);}
              break;

              case 'i' :
              if(last_i != 'x') {oop = i; exit(1);}
              break;
              
              case 'm' :
              if(last_i != 'i') {oop = i; exit(1);}
              break;
              
              default:
              i = -1;
              oop = -1;
              break;
            }

            last_i = i;
            count++;


 //           while( (port_putc( i, port ) != 0) /*&& (err == 0)*/ );
   //           testDELAY(200);

           // if ( err != 0 ) putchar(err);


    }

////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
