#ifndef PORT_C
#define PORT_C

#include <intrinsics.h> // MAXQ2000 specific stuff
#include <iomaxq200x.h> // ditto
#include "port.h"
#include "interface.c"
#include "RT99reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// This static variable com is
// the pointer to the port that will be serviced by the ISR.  
static PORT *com = NULL;

// This routine opens an RS-232 port up.  This means it
// allocates space for a PORT strcture, initializes the
// input and output buffers.
PORT *port_open( int address, int int_number )
{
//  unsigned char temp;
  PORT *port;
port = malloc( sizeof( PORT ));
  if ( port == NULL) return( NULL );
  com = port;
  port->in.tail = port->in.head =  0;
  port->out.tail = port->out.head = 0;
  port->uart_base = address;
  return( port );
}

// This routine establishes the operating parameters for a
// port after it has been opened.  This means it sets the
// baud rate, parity, number of data bits, and number of
// stop bits.  Interrupts are disabled before the routine
// starts changing registers, and are then reenabled 
// (if needed) after the changes are complete.
void port_set( PORT *port,
               long speed,
               char parity,
               int data,
               int stopbits, 
               int fifo)
{
  unsigned char lcr_out;
  unsigned char low_divisor;
  unsigned char high_divisor;

  POR(); 
  IFACE_Init();

  // Write to the Manual Reset reg and make sure that the RT99
  // registers are in default configuration.
  // This shouldn't affect the DLL and DLM
//  wreg( port->uart_base + EFR, EFR_RESET );
//  while ( (rreg( port->uart_base + EFR) & EFR_RESET)!=0 );
  
  // First disable all interrupts from the port.  I also read
  // RBR just in case there is a char sitting there ready to
  // generate an interupt.
  wreg( port->uart_base + IER, 0 );
  rreg( port->uart_base );

  // Enable or Disable FIFO
  wreg( port->uart_base + FCR, fifo );

  // Writing the baud rate means first enabling the divisor
  // latch registers, then writing the 16 bit divisor int
  // two steps, then disabling the divisor latch so the other
  // registers can be accessed normally.
  low_divisor = (char) (230400L / speed ) & 0xff;
  high_divisor = (char) ((230400L /  speed ) >> 8);
  wreg( port->uart_base + LCR, LCR_DLAB ); // Set DLAB = 1
  wreg( port->uart_base + DLL, low_divisor );
  wreg( port->uart_base + DLM, high_divisor );
  wreg( port->uart_base + LCR, 0 ); // Set DLAB = 0


  // Setting up the line control register establishes the
  // parity, number of bits, and number of stop bits.
  if ( parity== 'E' )
    lcr_out = LCR_EVEN_PARITY;
  else if ( parity == 'O' )
    lcr_out = LCR_ODD_PARITY;
  else if ( parity == 'M' )
    lcr_out = LCR_MARK_PARITY;
  else if ( parity == 'S' )
    lcr_out = LCR_SPACE_PARITY;
  else
    lcr_out = LCR_NO_PARITY;

  if ( stopbits == 2 )
    lcr_out |= LCR_2_STOP_BITS;

  if ( data == 6 )
    lcr_out |= LCR_6_DATA_BITS;
  else if ( data == 7 )
    lcr_out |= LCR_7_DATA_BITS;
  else if ( data == 8 )
    lcr_out |= LCR_8_DATA_BITS;

  wreg( port->uart_base + LCR, lcr_out ); // Set Byte config

//  mcr_out = MCR_RTS;
  wreg( port->uart_base + MCR, 0x00 ); // Clear Loopback

#ifdef INT_DRIVEN
  // Finally, restart interrupts, and exit.
   PD0   = 0x00; // Set port0 as input
   EIES0 = 0x01; // Set up falling edge trigger
   EIE0  = 0x01; // enable interrupt 0
   IMR  |= 0x01; // enable interrupts for module 1

   // Disables the TX Int and leave the RX Int enabled
//   wreg( port->uart_base + IER, IER_RX_DATA );
   wreg( port->uart_base + IER, 0x0F );

   __enable_interrupt();
#endif // INT_DRIVEN
}

// In order to close the port, I first disable interrupts
// ( if used) at the UART, then disable interrupts from 
// the UART's IRQ line. RTS is dropped. Then, the port 
// data structure is freed.
void port_close( PORT *port )
{
    __disable_interrupt();
   EIE0  = 0x00;  // disable interrupt 0
   IMR  |= 0x00;  // disable interrupts for module 1
   wreg( port->uart_base + IER, 0 );
   wreg( port->uart_base + MCR, 0 );
   wreg( port->uart_base + EFR, 1 ); // Manual Reset RT99
   free( port );
}

/////////////////////////////////////////////////////////////////
///////////////// Interrupt Functions ///////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef INT_DRIVEN

// This is the interrupt service routine for the COM port.
// It sits in a loop reading the interrrupt ID register, then
// servicing one of the five different types of 16550 interrupts.
#pragma vector = 0
__interrupt void interrupt_service_routine()
{
  unsigned char c;
  unsigned char val;
  

  if (EIF0 == 0x01) 
  {
  while(1) // Stay in loop while a valid interrupt exists
    {
      switch ( (rreg( com->uart_base + IIR )) & 0x0F ) // Check the LSB of the IIR
      {
        // If the present interrupt is due to a modem status line
        // change, the MSR is read to clear the interrupt, but
        // nothing else is done.
        case IIR_MODEM_STATUS :
          val = rreg( com->uart_base + MSR );
          //putchar('m');
          break;

        // If the interrupt is due to the transmit holding register
        // being ready for another character, I first check to see
        // if any characters are left in the TX buffer.  If
        // not, Transmit interrupts are disabled.  Otherwise, the
        // next character is extracted from the buffer and written
        // to the UART.
        case IIR_TRANSMIT :
          if ( !buf_empty(&com->out) )
          {
            c = com->out.buffer[ com->out.head++ & (BUFSIZ - 1) ];
            wreg( com->uart_base + THR, c );
          }
          else 
          {
            wreg( com->uart_base + IER, IER_RX_DATA ); 
          }
          break;

        // When a new character comes in and generates an
        // interrupt, it is read in.  If there is room in the input
        // buffer, it is stored, otherwise it is discarded.
        case IIR_RECEIVE :
          c = (unsigned char) rreg( com->uart_base + RBR );
          if ( (com->in.tail - com->in.head) & ~(BUFSIZ - 1) == 0 )
            com->in.buffer[ com->in.tail++ & (BUFSIZ - 1) ] =  c ;
          break;

        // All this code does is read the Line Status register, to
        // clear the source of the interrupt.
        case IIR_LINE_STATUS :
          val = rreg( com->uart_base + LSR );
          switch( val & 0x1E )
          {
            case 0: 
              break;
            case LSR_OE: 
              puts( "OE" );
              break;
            case LSR_PE: 
              puts( "PE" );
              break;
            case LSR_FE: 
              puts( "FE" ); 
              break;
            case LSR_BI: 
              puts( "BI" );
              break;
          }
          break;

        // All this code does is read the Receive Buffer register, to
        // clear the source of the interrupt.
        case IIR_CHAR_TIMEOUT :
          rreg( com->uart_base + RBR );
putchar('o');
          break;

        // If there are no valid interrupts left to service, an EOI
        // is written to the 8259 interrupt controller, and the
        // routine exits.
         case 0x01:
//          putchar('1');
          EIF0 = 0x00;
          return;
        case 0x03:
          putchar('z');
          EIF0 = 0x00;
          return;
        case 0x05:
          putchar('5');
          EIF0 = 0x00;
          return;
        case 0x07:
          putchar('7');
          EIF0 = 0x00;
          return;
        case 0x09:
          putchar('9');
          EIF0 = 0x00;
          return;
        case 0x0B:
          putchar('B');
          EIF0 = 0x00;
          return;
        case 0x0D:
          putchar('D');
          EIF0 = 0x00;
          return;
        case 0x0F:
          putchar('F');
          EIF0 = 0x00;
          return;
        default:
          putchar('X');
          EIF0 = 0x00;
          return;
      } // end switch()
    } // end while()
  } // end if()
  else return;
} // end ISR

// This routine is used to send a single character out to
// the UART.  If there is room in the output buffer, the
// character is inserted in the buffer.  Then the routine
// checks to see if Transmit interrupts are presently
// enabled for this UART.  If they aren't, they are turned
// on so the ISR will see this new character.
int port_putc( unsigned char c, PORT *port )
{
 // while ( buf_full(&port->out) );
    if ( buf_len(&port->out) >= BUFSIZ )
        return(-1);
    port->out.buffer[ port->out.tail++ & (BUFSIZ - 1)] = c;
    if (( rreg( port->uart_base + IER ) & IER_THRE) == 0 )
        wreg( port->uart_base+IER, IER_THRE | IER_RX_DATA );
    return( c );
}

// This routine checks to see if there is a character
// available in the input buffer for the specified port.
// If there is, it is pulled out and returned to the
// caller.
int port_getc(PORT *port)
{
  if ( buf_empty(&port->in) )
    return( -1 );
 else
    return( port->in.buffer[ port->out.head++ & (BUFSIZ - 1) ] );
}

#endif // INT_DRIVEN
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

//#else

/////////////////////////////////////////////////////////////////
///////////////// Interrupt Functions ///////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef POLLED

// PLACE polled FUNCTIONS HERE


// This routine is used to send a single character out to
// the UART using polling. If the transmitter is empty a
// character is written to the THR register.  
int port_putc( unsigned char c, PORT *port )
{
/*  if ( !buf_full(&port->out) )
  {
    push_tail(&port->out);
    port->out.buffer[ port->out.tail ] = c;
  }*/

  // wait here until the TEMT bit of the LSR is set.
  while ( (rreg( port->uart_base + LSR ) & LSR_TEMT ) == 0 );
  // write the character to the THR.
  wreg( port->uart_base, c );
return(1); // Temporary
}

// This routine checks to see if there is a character
// available in the input buffer for the specified port.
// If there is, it is pulled out and returned to the
// caller.
int port_getc(PORT *port)
{
  while ( (rreg( port->uart_base + LSR ) & LSR_DR) == 0 );
  return rreg( port->uart_base + RBR );
//  c = (unsigned char) rreg( port->uart_base + RBR );
//  if ( !buf_full(&port->in) )
//  {
//     push_tail( &port->in );
//     port->in.buffer[ port->in.tail ] =  c ;
//  }
//  if (buf_empty(&port->in))
//    return( -1 );
//  else
//    return( port->in.buffer[ pop_head( &port->in ) ] );
return(1);
}
#endif // POLLED
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#endif
