//
//  File........: TC0700.c
//  Authors.....: Tom Tripp
//  Description.: This is a test case that uses the SPI interface to xmit a string of characters at 38,400, 
//  8N1 with FIFOs disabled
//
//  Conditions:
//    IFACE:  SPI or I2C
//    BAUD:    bps
//    WORD:   8, N, 1
//    FIFO:   Yes or No
//    INTS:   Yes or No
//
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c


#ifndef TC0700_C
#define TC0700_C

// Interrupt or not? - define only one.
#define INT_DRIVEN
//#define POLLED

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

#define BPS230400 0x0001
#define BPS11520  0x0002
#define BPS76800  0x0003
#define BPS57600  0x0004
#define BPS38400  0x0006
#define BPS19200  0x000C
#define BPS9600   0x0018
#define BPS4800   0x0030
#define BPS2400   0x0060
#define BPS1200   0x00C0
#define BPS907    0x00FE //907.0866
#define BPS903    0x00FF //903.5294
#define BPS900    0x0100
#define BPS300    0x0300
#define BPS150    0x0600
#define BPS75     0x0C00
#define BPS57     0x0FA5 //57.52809
#define BPS37     0x1800 //37.5
#define BPS28     0x1F5A //28.7067
#define BPS18     0x3000 //18.75
#define BPS9      0x6000 //9.375
#define BPS6      0x8778 //6.643599
#define BPS4      0xC000 //4.6875
#define BPS3_8    0xEB64 //3.823432
#define BPS3_75   0xF000 //3.75
#define BPS3_5157 0xFFFE //3.515732
#define BPS3_5156 0xFFFF //3.515679





main()
{
///// Declare Variables Here ///////////////////////


////////////////////////////////////////////////////
    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
///// Your Code Here //////////////////////////////





/*
for( i = 0 ; string[i] != 0 ; i++)
{
  while( rreg(MSR) & 0x10 );        // Loop until the CTS ready
  wreg( THR, (int)string[i] );
}
*/


////////////////////////////////////////////////////
    port_close( port );
    return( 0 );
} // End of main(void)

#endif
