// SPI.h
#include <intrinsics.h>
#include <iomaxq200x.h>

//#define NEG_LEVEL_INT
#define POS_PULSE_INT
//#define NEG_PULSE_INT

__no_init volatile __io unsigned short CKCN @ _M(8,0xE);  /* System Clock Control */ 
__no_init volatile __io unsigned short IMR @ _M(8,0x6);   /* Interrupt Mask Register */ 

typedef unsigned char BYTE;
typedef unsigned short WORD;

typedef struct
	{
	BYTE	CMD;
	BYTE	WrLen;
	BYTE	RdLen;
	BYTE	*pRdD;
	BYTE	*pWrD;
	}SPI_PKT; 


// SPIfuncs.c Prototypes
void readbytes(BYTE reg, BYTE N, BYTE *p);
void writebytes(BYTE reg, BYTE N, BYTE *p);
void Reset_MAX3410E(WORD);
void initIO(void); 
void Ack_Status(void);
void wreg(BYTE, BYTE);
BYTE Check_INT(void);
void wreg(BYTE,BYTE);
void wregAS(BYTE,BYTE);
unsigned int rreg(unsigned int);
BYTE rregAS(BYTE);
BYTE SPI_wtr(SPI_PKT *p);		// SPI Write-Then-Read
BYTE readstat(void);

// Enumeration stuff
void main(void);
void set_addr(void);
void std_request(void);
void class_request(void);
void vendor_request(void);
void send_descriptor(void);
void feature(BYTE);
void get_status(void);
void set_interface(void);
void get_interface(void);
void set_configuration(void);
void get_configuration(void);
// Application stuff
void send_keystroke(BYTE);
	
// Macros
//
#define CS_HI PO5 |= 0x10;
#define CS_LO PO5 &= ~0x10;

#define L1_ON wreg(rGPIO,(rreg(rGPIO) & 0xFE));
#define L1_OFF wreg(rGPIO,(rreg(rGPIO) | 0x01));
#define L2_ON wreg(rGPIO,(rreg(rGPIO) & 0xFD));
#define L2_OFF wreg(rGPIO,(rreg(rGPIO) | 0x02));
#define L3_ON wreg(rGPIO,(rreg(rGPIO) & 0xFB));
#define L3_OFF wreg(rGPIO,(rreg(rGPIO) | 0x04));
#define L4_ON wreg(rGPIO,(rreg(rGPIO) & 0xF7));
#define L4_OFF wreg(rGPIO,(rreg(rGPIO) | 0x08));
 
#define SUSPEND_INT_ON wreg(rUSBIEN,(rreg(rUSBIEN) | bmSUSPEND));
#define SUSPEND_INT_OFF wreg(rUSBIEN,(rreg(rUSBIEN) & !bmSUSPEND));
#define BUSACT_INT_ON wreg(rUSBIEN,(rreg(rUSBIEN) | bmBUSACT));
#define BUSACT_INT_OFF wreg(rUSBIEN,(rreg(rUSBIEN) & !bmBUSACT));
#define SETBIT(reg,val) wreg(reg,(rreg(reg)|val));
#define CLRBIT(reg,val) wreg(reg,(rreg(reg)&~val));
//
#define STALL_EP0 wreg(9,0x23);	// Set all three EP0 stall bits--data stage IN/OUT and status stage
#define MSB(word) (BYTE)(((WORD)(word) >> 8) & 0xff)
#define LSB(word) (BYTE)((WORD)(word) & 0xff)

// MAX3410E Registers
#define rEP0FIFO 0
#define rEP1OUTFIFO	1
#define rEP2INFIFO 2
#define rEP3INFIFO 3
#define rSUDFIFO 4
#define rEP0BC 5
#define rEP1OUTBC 6
#define rEP2INBC 7
#define rEP3INBC 8
#define rEPSTALLS 9
#define rCLRTOGS 10
#define rEPIRQ 11
#define rEPIEN 12
#define rUSBIRQ 13
#define rUSBIEN 14
#define rUSBCTL 15
#define rCPUCTL 16
#define rPINCTL 17
#define rRevision 18
#define rFNADDR	19
#define rGPIO 20

// Constants
// PINCTL Register
#define bmFDUPSPI 0x10
#define bmINTLEVEL 0x08
#define bmPOSINT 0x04
#define bmGPOB 0x02
#define	bmGPOA 0x01
// USBCTL Register
#define bmHOSCSTEN 0x80
#define bmVBGATE 0x40
#define bmCHIPRES 0x20
#define bmPWRDOWN 0x10
#define bmCONNECT	0x08
#define bmRESUME 0x04
// R11 EPIRQ/EPIEN register bits
#define bmSUDAV 0x20
#define bmIN3BAV 0x10
#define bmIN2BAV 0x08
#define bmOUT1DAV 0x04
#define bmOUT0DAV 0x02
#define bmIN0BAV 0x01
// R12 USBIRQ/USBIEN register bits
#define bmUSBRESDN 0x80
#define bmVBUS 0x40
#define bmNOVBUS 0x20
#define bmSUSPEND 0x10
#define bmUSBRES 0x08
#define bmBUSACT 0x04
#define bmRSUMDN 0x02
#define bmOSCOK 0x01
// R9 EPSTALLS register bits
#define bmACKSTAT 0x40
#define bmCTLSTAT 0x20
#define bmEP3IN 0x10
#define bmEP2IN 0x08
#define bmEP1OUT 0x04
#define bmEP0OUT 0x02
#define bmEP0IN 0x01
// R16 CPUCTL bits
#define bmIE 0x01
// SPI status byte bits
#define stSUSPEND 0x80
#define stUSBRES 0x40
#define stSUDAV 0x20
#define stIN3BAV 0x10
#define stIN2BAV 0x08
#define stOUT1DAV 0x04
#define stOUT0DAV 0x02
#define stIN0BAV 0x01

// Standard Requests
#define SR_GET_STATUS			0x00	// Setup command: Get Status
#define SR_CLEAR_FEATURE		0x01	// Setup command: Clear Feature
#define SR_RESERVED				0x02	// Setup command: Reserved
#define SR_SET_FEATURE			0x03	// Setup command: Set Feature
#define SR_SET_ADDRESS			0x05	// Setup command: Set Address
#define SR_GET_DESCRIPTOR		0x06	// Setup command: Get Descriptor
#define SR_SET_DESCRIPTOR		0x07	// Setup command: Set Descriptor
#define SR_GET_CONFIGURATION	        0x08	// Setup command: Get Configuration
#define SR_SET_CONFIGURATION	        0x09	// Setup command: Set Configuration
#define SR_GET_INTERFACE		0x0a	// Setup command: Get Interface
#define SR_SET_INTERFACE		0x0b	// Setup command: Set Interface
// Get Descriptor codes	
#define GD_DEVICE			0x01	// Get device descriptor: Device
#define GD_CONFIGURATION		0x02	// Get device descriptor: Configuration
#define GD_STRING			0x03	// Get device descriptor: String
#define GD_HID	            	        0x21	// Get descriptor: HID
#define GD_REPORT	         	0x22	// Get descriptor: Report
// SETUP packet offsets
#define bmRequestType	0
#define	bRequest		1
#define wValueL			2
#define wValueH			3
#define wIndexL			4
#define wIndexH			5
#define wLengthL		6
#define wLengthH		7
// HID bRequest values
#define GET_REPORT		1
#define GET_IDLE		2
#define GET_PROTOCOL	        3
#define SET_REPORT		9
#define SET_IDLE		0x0A
#define SET_PROTOCOL	        0x0B
#define INPUT_REPORT	        1



