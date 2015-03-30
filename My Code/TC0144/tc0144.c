//
//  File........: TC0144.c
//  Authors.....: Tom Tripp
//  Description.: Test Loop Back MCR.4
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   57,600 bps
//    WORD:   8, N, 1
//    FIFO:   N + Y
//    INTS:   NO
//  Date........: March 2, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0144_C
#define TC0144_C

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
    const unsigned char tx_data[] = "Maxim - The quick brown fox came down with the pox.";
    unsigned char rx_data[50];


main()
{
///// Declare Variables Here ///////////////////////
    int i = 0;
    
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



/*
    // Disable FIFO
    wreg( port->uart_base + FCR,0x00);

    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xAA;
    }


    // Send data
    for( i=0; i<50; i++)
    {
      wreg( port->uart_base + THR,tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_DR) == 0);
      // Receive data
      rx_data[i] = rreg( port->uart_base + RBR); 
    }

    // Check data
    for( i=0; i<50; i++)
    {
      if(rx_data[i] != tx_data[i])
        puts(" ERROR w/o FIFO\n"); 
// uncomment these to see string xfer
//      putchar(' ');    
//      putchar(rx_data[i]);    
//      putchar(' ');    
    }

    putchar('\n');    
    
    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xFF;
    }
*/

    // Enable FIFO
//    wreg( port->uart_base + FCR,0x01);
rreg( port->uart_base + RBR);
rreg(port->uart_base + LSR);

    for( i=0; i<=50; i++)
    {
      wreg( port->uart_base + THR,tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_DR) == 0);
      // Receive data
      rx_data[i] = rreg( port->uart_base + RBR); 
    }

    // Check data
    for( i=0; i<50; i++)
    {
      if(rx_data[i] != tx_data[i])
        puts(" ERROR w/FIFO\n"); 
// uncomment these to see string xfer
//      putchar(i+48);    
//      putchar(' ');    
//      putchar(rx_data[i]);    
//      putchar(' ');    
    }

    // Check data
    for( i=0; i<50; i++)
    {
// uncomment these to see string xfer
      putchar(' ');    
      putchar(tx_data[i]);    
       putchar('=');    
     putchar(rx_data[i]);    
      putchar(' ');    
    }
      putchar('\n');    


/*

    // Disable FIFO
    wreg( port->uart_base + FCR,0x00);

    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xAA;
    }


    // Send data
    for( i=0; i<50; i++)
    {
      wreg( port->uart_base + THR,tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_DR) == 0);
      // Receive data
      rx_data[i] = rreg( port->uart_base + RBR); 
    }

    // Check data
    for( i=0; i<50; i++)
    {
      if(rx_data[i] != tx_data[i])
        puts(" ERROR w/o FIFO\n"); 
// uncomment these to see string xfer
//      putchar(' ');    
//      putchar(rx_data[i]);    
//      putchar(' ');    
    }

    putchar('\n');    
    
    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xFF;
    }



    // Enable FIFO
    wreg( port->uart_base + FCR,0x01); // M
    for( i=0; i<=50; i++)
    {
      wreg( port->uart_base + THR,tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_DR) == 0);
      // Receive data
      rx_data[i] = rreg( port->uart_base + RBR); 
    }

    // Check data
    for( i=0; i<50; i++)
    {
      if(rx_data[i] != tx_data[i])
        puts(" ERROR w/FIFO\n"); 
// uncomment these to see string xfer
//      putchar(i+48);    
//      putchar(' ');    
//      putchar(rx_data[i]);    
//      putchar(' ');    
    }

    // Check data
    for( i=0; i<50; i++)
    {
// uncomment these to see string xfer
      putchar(' ');    
      putchar(rx_data[i]);    
       putchar(' ');    
     putchar(tx_data[i]);    
      putchar(' ');    
    }
      putchar('\n');    





    // Disable FIFO
    wreg( port->uart_base + FCR,0x00);

    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xAA;
    }


    // Send data
    for( i=0; i<50; i++)
    {
      wreg( port->uart_base + THR,tx_data[i]); // send a Byte
      while( (rreg(port->uart_base + LSR) & LSR_DR) == 0);
      // Receive data
      rx_data[i] = rreg( port->uart_base + RBR); 
    }

    // Check data
    for( i=0; i<50; i++)
    {
      if(rx_data[i] != tx_data[i])
        puts(" ERROR w/o FIFO\n"); 
// uncomment these to see string xfer
//      putchar(' ');    
//      putchar(rx_data[i]);    
//      putchar(' ');    
    }

    putchar('\n');    
    
    for( i=0; i<50; i++)
    {
      rx_data[i] = 0xFF;
    }
    puts( " End of Test!\n");
*/
while(1);


//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
