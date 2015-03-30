//
//  File........: TC0001.c
//  Authors.....: Bob Kelly/Tom Tripp
//  Description.: This is a test case that uses the SPI interface to print out the value of the RT99 RCR register.
//  
//  Conditions:
//    IFACE:  SPI
//    BAUD:   N/A bps
//    WORD:   N/A
//    FIFO:   No
//    INTS:   No
//  Date........: December 10, 2004
//
// The proper interface can be found in interface.c

#ifndef TC0001_C
#define TC0001_C

// Interrupt or not? - define only one.
//#define INT_DRIVEN
//#define POLLED


// Which interface? - define only one.
//#define IFACE_SPI	0
#define IFACE_I2C	1

//#include <stdio.h>
//#include <stdlib.h>
#include <ezusb.h>
#include <ezregs.h>
#include "RT99reg.h"      // RT99 Registers and bits
#include "interface.c"    // SPI/I2C Implementation
#include "port.c"         // RS232/16550 Implementation


BYTE xdata reg_addr = 0x07;

main()
{
///// Declare Variables Here ///////////////////////
//unsigned char x[8];
int i;
////////////////////////////////////////////////////
/*    PORT *port;
    port = port_open( COM1_UART, COM1_INTERRUPT );
    // Check to see if 
    if ( port == NULL ) 
    {
        printf( "Failed to open the port!\n" );
        exit( 1 );
    }
    port_set( port, 38400L, 'N', 8, 1, NO_FIFO );
*//// Your Code Here //////////////////////////////

//  POR();
  IFACE_Init();

//for(i=0;i<=7;i++)
//{
wreg(reg_addr, 0x14);
rreg(reg_addr);
// x[i] = rreg(RCR);
//  printf("RCR: 0x%X\n", x );
//testDELAY(140);
//}
/*
for(i=0;i<=7;i++)
{
// x = rreg(RCR);
//  printf("RCR: %X\n", x[i] );
putchar(x[i]);
putchar('\n');
}
*/
while(1);
////////////////////////////////////////////////////
//    port_close( port );
    return( 0 );
} // End of main(void)


#endif



/*

// #define TNG
// #define RT99_ADDR		0x1C

// P20 off
// P21 off
// P22 on
//const BYTE xdata reg_addr = 0x0D;
//BYTE xdata reg_val;



main()
{
//	int i;
//	BYTE x;
//	EZUSB_InitI2C(); // Initialize EZ-USB I2C controller

	for(i=0;i<=7;i++)
{
	rreg(RCR);
//	EZUSB_WriteI2C(RT99_ADDR, 0x01, &reg_addr);
//	x = EZUSB_ReadI2C(RT99_ADDR,0x01,&reg_val);

}
	while(1);
} // End of Main()
*/
