///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This portion contains the structure definitions, constants,
// and function prototypes needed to use the RS-232
// interface code supplied in interface.C.  It should be included
// by any routine using interface.C procedures.

#ifndef INTERFACE_H
#define INTERFACE_H

__no_init volatile __io unsigned char PO5 @ _M(1,1);   // Port 5 Output Register 


//typedef unsigned char BYTE;
//typedef unsigned short WORD;

// Function prototypes.  All of these
// routines actually reside in interface.C
//void testDELAY(int);
unsigned short int sendBYTE( unsigned short int );
unsigned short int rreg( unsigned short int );
void wreg( unsigned short int, unsigned char );
void IFACE_Init(void);
// void testDELAY(int);


#endif // INTERFACE_H
