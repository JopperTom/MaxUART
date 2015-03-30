///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This portion contains the structure definitions, constants,
// and function prototypes needed to use the RS-232
// interface code supplied in port.C.  It should be included
// by any routine using port.c procedures.


#ifndef PORT_H
#define PORT_H

#define BUFSIZ 16

// This structure defines the 256 byte buffer used for
// I/O buffers by the COM routines.  By using a buffer
// size of 256 bytes, updating the indices is simplified.
typedef struct {
                 char buffer[BUFSIZ];
                 unsigned char tail;
                 unsigned char head;
               } BUFFER;

// This structure defines a COM port.  It is initialized
// when the port is opened with port_open().
typedef struct {
                 int                   uart_base;
                 BUFFER                in;
                 BUFFER                out;
} PORT ;

PORT *port_open( int address, int interrupt_number );
void port_set( PORT *port,
               long speed,
               char parity,
               int data,
               int stopbits,
               int fifo );
void port_close( PORT *port );
int port_putc( unsigned char c, PORT *port );
int port_getc( PORT *port );

short int buf_empty(BUFFER *b)
{
    return (b->head == b->tail);
}

unsigned char buf_len(BUFFER *b)
{
  return ( b->tail - b->head );
}
 #define COM1_UART         0x000
 #define COM1_INTERRUPT    0
 #define COM2_UART         0x001
 #define COM2_INTERRUPT    1

#endif // PORT_H

