//
//  File........: TC0147.c
//  Authors.....: Tom Tripp
//  Description.: Test Packet formats
//
//  Conditions:
//    IFACE:  SPI 
//    BAUD:   115200 bps
//    WORD:   all
//    FIFO:   NO
//    INTS:   NO
//  Date........: February 28, 2005
//
// The proper interface can be found in interface.c

#ifndef TC0151_C
#define TC0151_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
#define POLLED

// Which interface? - define only one.
#define IFACE_SPI
//#define IFACE_I2C

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <intrinsics.h>   // MAXQ2000 specific stuff
#include <iomaxq200x.h>   // MAXQ2000 specific stuff
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation
    int i, j ,k, temp;
    int m = 2;
    unsigned char tx_data;
    unsigned char rx_data;
    unsigned char parity[5] = {'N', 'E', 'O', 'M', 'S'};


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
    port_set( port, 230400L, 'N', 8, 1, NO_FIFO, FCR_RX_TRIG_1B );


/////// Your Code Here //////////////////////////////

  wreg(port->uart_base + MCR, MCR_LOOP_BACK);

    for( i=8; i >= 5; i--)
    {
 //      putchar(i+48);
       for(temp = i-1, m = 2; temp > 0; temp--)
        {
            m *= 2;
        }
        for( j = 1; j <= 2; j++)
        {
//            putchar('\n');
//            putchar(j+48);
//            putchar('\n');
            for( k=0; k < 5 ; k++)
            {
//                putchar(parity[k]);
                POR();
                port_set( port, 230400L, parity[k], i, j, NO_FIFO, FCR_RX_TRIG_1B );
                wreg(port->uart_base + MCR, MCR_LOOP_BACK);
                for( tx_data = 0; tx_data < m-1 ; tx_data++)
                {
                    wreg( port->uart_base + THR, tx_data); // send a Byte

                    while( (rreg(port->uart_base + LSR) & LSR_DR) == 0x00);
                    // Receive data
                    rx_data = rreg( port->uart_base + RBR); 
      
                    if( tx_data != rx_data)
                    {
                        //continue;
                        puts("error.");
                        exit(1);
                    } 
                }
            }     
        }  
//        putchar('\n');
    }
    


    // delay in order to complete output
    testDELAY(10000);
    puts(" End of Test.\n");

//////////////////////////////////////////////////////////  
    port_close( port );
    return( 0 );
}

#endif
