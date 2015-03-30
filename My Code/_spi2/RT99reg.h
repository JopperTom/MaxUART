#ifndef RT99REG_H
#define RT99REG_H


#define CS_HI // P05 |= 0x10;          // Macros to set the MAXQ2000 SPI CS signal (P54) high and low
#define CS_LO // P05 &= ~0x10;
#define SDA_HI                       // Macros to set the               to high and low
#define SDA_LO
#define SCL_HI 
#define SCL_LO
#define UVLO_HI //P07 |= 0x01;
#define UVLO_LO //P07 &= ~0x01;

//void testDELAY(int x){int y; for( y=0; y<x ; y++);}

// This group of defines creates all the definitions used
// to access registers and bit fields.
#define RBR               0x00    /* Receive buffer REGISTER      */
#define THR               0x00    /* Transmit holding REGISTER    */
#define IER               0x01    /* Interrupt Enable REGISTER    */
#define IER_RX_DATA       0x01      /* Enable RX interrupt BIT      */
#define IER_THRE          0x02      /* Enable TX interrupt BIT      */
#define IER_LINE_STATUS   0x04      /* Enable Line Status INT BIT   */
#define IER_MODEM_STATUS  0x08      /* Enable Modem Status INT BIT  */
#define IIR               0x02    /* Interrupt ID REGISTER        */
#define IIR_MODEM_STATUS  0x00      /* Modem stat. interrupt ID     */
#define IIR_TRANSMIT      0x02      /* Transmit interrupt ID        */
#define IIR_RECEIVE       0x04      /* Receive interrupt ID         */
#define IIR_LINE_STATUS   0x06      /* Line stat. interrupt ID      */
#define IIR_CHAR_TIMEOUT  0x0C      /* Char. Timeout interrupt ID   */
#define FCR               0x02    /* FIFO Control REGISTER        */
#define FCR_FIFO_ENABLE   0x01      /* Bit that enables the FIFO    */
#define FCR_RX_FIFO_RESET 0x02      /* Bit that resets the rx FIFO  */
#define FCR_TX_FIFO_RESET 0x04      /* Bit that resets the tx FIFO  */
#define FCR_RX_TRIG_LSB   0x40      /* Bit that sets the rx trigger */
#define FCR_RX_TRIG_MSB   0x80      /* Bit that sets the rx trigger */
#define LCR               0x03    /* Line control REGISTER        */
#define LCR_DLAB          0x80      /* Divisor access bit           */
#define LCR_EVEN_PARITY   0x18      /* Set parity 'E' bits          */
#define LCR_ODD_PARITY    0x08      /* Set parity 'O' bits          */
#define LCR_MARK_PARITY   0x28      /* Set parity 'M' bits          */
#define LCR_SPACE_PARITY  0x38      /* Set parity 'S' bits          */
#define LCR_NO_PARITY     0x00      /* Set parity 'N' bits          */
#define LCR_1_STOP_BIT    0x00      /* Bits to set 1 stop bit       */
#define LCR_2_STOP_BITS   0x04      /* Bits to set 2 stop bits      */
#define LCR_5_DATA_BITS   0x00      /* Bits to set 5 data bits      */
#define LCR_6_DATA_BITS   0x01      /* Bits to set 6 data bits      */
#define LCR_7_DATA_BITS   0x02      /* Bits to set 7 data bits      */ 
#define LCR_8_DATA_BITS   0x03      /* Bits to set 8 data bits      */
#define LCR_SET_BREAK     0x40      /* Bit to set a break cond.     */
#define MCR               0x04    /* Modem control REGISTER       */
#define MCR_RTS           0x02      /* Bit to turn on RTS           */
#define MCR_LOOP_BACK     0x10      /* Bit to turn on LoopBack      */
#define LSR               0x05    /* Line Status REGISTER         */
#define LSR_DR            0x01      /* Bit that shows RX Data Ready */
#define LSR_OE            0x02      /* Bit that shows Overrun Error */
#define LSR_PE            0x04      /* Bit that shows Parity Error  */
#define LSR_FE            0x08      /* Bit that shows Framing Error */
#define LSR_BI            0x10      /* Bit that shows Break Interrt */
#define LSR_THRE          0x20      /* Bit that shows THR empty     */
#define LSR_TEMT          0x40      /* Bit that shows TSR+THR empty */
#define LSR_FERR          0x80      /* Bit that shows RX FIFO error */
#define MSR               0x06    /* Modem Status REGISTER        */
#define MSR_DELTA_CTS     0x01      /* Bit that shows Delta in CTS  */
#define MSR_CTS           0x10      /* Bit that shows State of CTS  */
#define DLL               0x00    /* Divisor latch LSB            */
#define DLM               0x01    /* Divisor latch MSB            */
#define SCR               0x07    /* Scratch REGISTER             */
// Below here are not 16550 registers
#define xDLL              0x08    /* xtended Divisor Latch LSB Table 13.4.2.2   */
#define xDLM              0x09    /* xtended Divisor Latch MSB Table 13.4.2.2   */
#define NO_NAME           0x0A    /* Read LSR + RBR registers  Table 13.4.2.2   */
#define HCR               0x1F    /* Hardware Control REGISTER Table 13.4.2.2   */
#define TST               0x2F    /* Test REGISTER             Table 13.4.2.2   */
#define RCR               0x000D    /* Revision REGISTER            */
#define EFR               0x0E    /* Extended Function REGISTER   */
#define EFR_AUTO_CTS      0x80      /* Bit to turn on AutoCTS       */
#define EFR_AUTO_RTS      0x40      /* Bit to turn on AutoRTS       */
#define EFR_SPI_MODE      0x20      /* Bit to change SPI modes      */
#define EFR_RESET         0x01      /* Bit to turn on Manual Reset  */
#define HCR               0x1F    /* Hardware Control REGISTER    */
#define HCR_WAKE_ON_RX    0x20      /* Bit to turn on Wake on RX    */
#define HCR_WAKE          0x00      /* Bit to wake from sleep       */
#define HCR_SLEEP         0x40      /* Bit to enter sleep mode      */
#define HCR_SWSHDN        0xC0      /* Bit to SW Shutdown           */

#define TST               0x2F    /* Test REGISTER                */




// Various constants used only in this program.
//#define INT_CONTROLLER   0x20  /* The address of the 8259*/
//#define EOI              0x20  /* The end of int command */
//#define BREAK_VECTOR     0x23  /* The CTRL-BREAK vector  */
#define YES_FIFO          0x01    /* Enable FIFO             */
#define NO_FIFO           0x00    /* Disable FIFO            */

#endif
