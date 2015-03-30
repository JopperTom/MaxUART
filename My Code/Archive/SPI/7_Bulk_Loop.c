// 7_Bulk_Loop.c	7-14-04	LTH  
// Bulk Loop Test. Enumerate, then:
// Accept packets from EP1-OUT, send the packets back over EP2-IN.
// Send the the transfer count over EP3-IN.

#include <intrinsics.h>
#include <iomaxq200x.h>
#include "SPI.h"
#include "USB_APA_Data.h"	// so we can use Andrew's control panel
#define TIMEOUT 1500		// for set-reset LED pushbuttons

//Global variables
SPI_PKT pkt;			// SPI packet structure
BYTE SUD[8];			// my copy of setup data
BYTE msgidx,msglen;		// message index and length
BYTE configval;			// From Set_Configuration, reported back in Get_Configuration
BYTE ep1stall,ep2stall,ep3stall;	// flag for EP3 Stall, set by Set_Feature, reported back in Get_Status
BYTE interfacenum;
unsigned long total_bytes;	// count every byte transferred, IN or OUT
BYTE xfrmode;			// transfer mode. 0: EP1->EP2, 1:EP1->EP3 
//
// prototypes
void Initialize_RT86(void);
void do_SETUP(void);
void do_IN3(void);
// __interrupt void USBinterrupt(void);  // causes compile error if prototype checking is ON

void Initialize_RT86(void)	// call at power-on or USB Bus Reset
{
// Enable selected MAX3410E interrupts
ep1stall=0;
ep2stall=0;
ep3stall=0;
wreg(rGPIO,0xFF);			// Lites off
//wreg(rEPIEN,(bmSUDAV | bmOUT1DAV));	// Enable the SUDAV & OUT1 IRQS
//wreg(rUSBIEN,(bmUSBRES|bmUSBRESDN));	// These are not cleared during USB bus reset
wreg(rUSBIEN,bmBUSACT);   // DEBUG--only one
}
/*
#pragma vector=1
__interrupt void USBinterrupt(void)
{
  EIF1_bit.IE12 = 0;                    // clear the interrupt request flag
  gotUSB = 1;
}
*/
// ****************************** MAIN ********************************
void main(void)
{
BYTE itest1,itest2,blink;	        // for polling the interrupt requests
BYTE dat[64];				// Loopback data
BYTE xfr_bc;				// bytes to transfer in the packet
WORD button_time;
configval=0;				// initially UNCONFIGURED
xfrmode=2;				// initially transfer EP1->EP2
//
initIO();
Reset_RT86(50000);			// for 1000 time units
Initialize_RT86();

// For testsing the three interrupt pin modes:
#ifdef NEG_LEVEL_INT
wreg(rPINCTL,(bmFDUPSPI|bmINTLEVEL));	// INTLEVEL=1 (level), POSINT=X in level mode (always neg)
#endif
#ifdef POS_PULSE_INT 
wreg(rPINCTL,(bmFDUPSPI|bmPOSINT));	// INTLEVEL=0 (pulse), POSINT=1 (posedge edge)
#endif
#ifdef NEG_PULSE_INT
wreg(rPINCTL,bmFDUPSPI);	// INTLEVEL=0 (pulse), POSINT=0 (negative edge)
#endif

wreg(rUSBCTL,bmCONNECT);	// VBGATE=0 so CONNECT is not conditional on Vbus present
wreg(rCPUCTL,bmIE);		// Enable interrupt pin
total_bytes=0;
L1_ON
L1_OFF
L2_ON
L2_OFF
L3_ON
L3_OFF
L4_ON
L4_OFF

while(1)				// endless loop
  {
/*
    do	
	{
	button_time++;
	if (button_time == TIMEOUT)
	  {
	      button_time = 0;
			if((rreg(rGPIO)&0x80)==0)			// check for button3 press
			{
			  CLRBIT(rUSBCTL,bmCONNECT);		// disconnect from USB
			  while((rreg(rGPIO)&0x80)==0) ;	// hang until button released
			  SETBIT(rUSBCTL,bmCONNECT);		// reconnect
			}

	      blink++;
                if (blink==128)
		  L1_ON
		else if (blink==0)
		  L1_OFF
	  } // if (button_time == TIMEOUT)
	} // do
*/
      while(PI3&0x80);    // P30 is high if no interrupt (SET TO LEVEL-LOW)

//    while (Check_INT() == 0);		// while no pending USB interrupts, just blink the light
	  itest1 = rreg(rEPIEN) & rreg(rEPIRQ);	    // only consider the enabled ones
	  itest2 = rreg(rUSBIEN) & rreg(rUSBIRQ);   // ditto
          
// Something is pending. The order of the if statements below sets the "priority".
	if(itest1 & bmSUDAV) 
		{
		wreg(rEPIRQ,bmSUDAV);	// clear the SUDAV IRQ
		do_SETUP();
		}

        else if (itest2 & bmBUSACT)
          wreg(rUSBIRQ,bmBUSACT);    // just clear the IRQ
 
	else if(itest1 & bmOUT1DAV)		
		{											// Just received an OUT packet over EP1-OUT
			if (xfrmode==2)
			{
				if (rreg(rEPIRQ) & bmIN2BAV)	// Is the correct INPUT buffer available?
				{								// YES--Read in a burst for best performance
				// read the bytes in EP1OUTFIFO into dat array (holds 64 bytes)
				xfr_bc		= 	rreg(rEP1OUTBC);
                                readbytes(rEP1OUTBC,xfr_bc,dat);
				wreg(rEPIRQ,bmOUT1DAV);				// clear the IRQ and arm EP1OUT
				total_bytes += xfr_bc;

				// load the bytes in dat[] into EP2INFIFO
                                writebytes(rEP2INFIFO,xfr_bc,dat);
				wreg(rEP2INBC,xfr_bc);				// arm EP2-IN and clear the IRQ
				total_bytes += xfr_bc;						
				} 
//				else 								// NO, an input buffer is not available. Enable IN2BAV INT so we can tell when it is.
//					SETBIT(rEPIEN,bmIN2BAV);
			}
			else if (xfrmode==3)
			{
				if (rreg(rEPIRQ) & bmIN3BAV)	// Is the correct INPUT buffer available?
				{								// YES--Read in a burst for best performance
				// read the bytes in EP1OUTFIFO into dat array (holds 64 bytes)
				xfr_bc		= 	rreg(rEP1OUTBC);
                                readbytes(rEP1OUTBC,xfr_bc,dat);
				wreg(rEPIRQ,bmOUT1DAV);				// clear the IRQ and arm EP1OUT
				total_bytes += xfr_bc;

				// load the bytes in dat[] into EP3INFIFO
                                writebytes(rEP3INFIFO,xfr_bc,dat);
				wreg(rEP3INBC,xfr_bc);				// arm EP3-IN and clear the IRQ
				total_bytes += xfr_bc;
				} 
//				else 						// NO, an input buffer is not available. Enable IN2BAV INT so we can tell when it is.
//					SETBIT(rEPIEN,bmIN3BAV);
			}
		}
		
//	Since this program enumerates we need to handle USB bus reset, which clears most interrupt enables

	else if(itest2 & bmUSBRES)		
		{
		wreg(rUSBIRQ,bmUSBRES);				// clear the IRQ
		L4_ON						// green light
		}
	else if(itest2 & bmUSBRESDN)		
		{
		wreg(rUSBIRQ,bmUSBRESDN);			// clear the IRQ
		Initialize_RT86();				// start over--re-enable the interrupts
		L4_OFF						// green light
		}
	} // end while(1)
} // end main

void do_SETUP(void)
{							// got a SETUP packet. Read 8 SETUP bytes
// Read the 8 SETUP bytes in a burst (one CS interval for 8 read bytes)

readbytes(rSUDFIFO,8,SUD);
					// Parse the SETUP packet
switch(SUD[bmRequestType]&0x60)		// look only at b6&b5
	{
	case 0x00:	std_request();		break;
	case 0x20:	class_request();	break;
	case 0x40:	vendor_request();	break;
	default:	STALL_EP0
	}
}

// ********************************************************************************************************
void std_request(void)
{
switch(SUD[bRequest])			
	{
	case	SR_GET_DESCRIPTOR:	send_descriptor();	break;
	case	SR_SET_FEATURE:		feature(1);		break;
	case	SR_CLEAR_FEATURE:	feature(0);		break;
	case	SR_GET_STATUS:		get_status();		break;
	case	SR_SET_INTERFACE:	set_interface();	break;
	case	SR_GET_INTERFACE:	get_interface();	break;
	case	SR_GET_CONFIGURATION: 	get_configuration();	break;
	case	SR_SET_CONFIGURATION: 	set_configuration();	break;
	case	SR_SET_ADDRESS:		set_addr();             break;
	default:STALL_EP0
	}
}

void set_addr(void)
{
//rregAS(rRevision);
wregAS(rEPSTALLS,0x00);   // harmless write
}

void set_configuration(void)
{
configval=SUD[wValueL];	// Config value is here
rregAS(rFNADDR);		// dummy read to set the ACKSTAT bit
}

void get_configuration(void)
{
wreg(rEP0FIFO,configval);
wregAS(rEP0BC,1);
}

// *******************************************************************************************
void set_interface(void)		// All we accept are Interface=0 and AlternateSetting=0, otherwise send STALL
{
if((SUD[wValueL]==0)		// wValueL=Alternate Setting index
  &(SUD[wIndexL]==0))		// wIndexL=Interface index
  	rregAS(rFNADDR);		// dummy read to set the ACKSTAT bit
else STALL_EP0
}

void get_interface(void)		// Check for Interface=0, always report AlternateSetting=0
{
if(SUD[wIndexL]==0)			// wIndexL=Interface index
  	{
	wreg(rEP0FIFO,0);			// AS=0
	wregAS(rEP0BC,1);			// send one byte, ACKSTAT
	}
else STALL_EP0
}

// *******************************************************************************************
void get_status(void)
{
BYTE testbyte,ep;
testbyte=SUD[bmRequestType];
ep=SUD[wIndexL];				// endpoint number
switch(testbyte)	
	{
	case 0x80: 					// directed to DEVICE
		wreg(rEP0FIFO,0x00);			// first byte is 000000rs where r=enabled for RWU and s=self-powered.
		wreg(rEP0FIFO,0x00);			// second byte is always 0
		wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
		break; 				
	case 0x81: 					// directed to INTERFACE
		wreg(rEP0FIFO,0x00);			// this one is easy--two zero bytes
		wreg(rEP0FIFO,0x00);		
		wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
		break; 				
	case 0x82: 					// directed to ENDPOINT
		if(ep==0x01)				// EP1-OUT = 0x01
			{
			wreg(rEP0FIFO,ep1stall);		// first byte is 0000000h where h is the halt (stall) bit
			wreg(rEP0FIFO,0x00);			// second byte is always 0
			wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
			}
		else if(ep==0x82)			// EP2-IN = 0x82
			{
			wreg(rEP0FIFO,ep2stall);		// first byte is 0000000h where h is the halt (stall) bit
			wreg(rEP0FIFO,0x00);			// second byte is always 0
			wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
			}
		else if(ep==0x83)			// EP3-IN = 0x83
			{
			wreg(rEP0FIFO,ep3stall);		// first byte is 0000000h where h is the halt (stall) bit
			wreg(rEP0FIFO,0x00);			// second byte is always 0
			wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
			}
		else 	STALL_EP0					// not a valid EP
	default:	STALL_EP0				// not a valid target
	}
}

// *******************************************************************************************
// FUNCTION: call as feature(1) for Set_Feature or feature(0) for Clear_Feature.
// There are two set/clear feature requests:
//		To a DEVICE: 	Remote Wakeup (RWU)
//  	To an ENDPOINT:	stall
// The descriptor for this device reports not RWU-capable, so we should never see the first one.
//
void feature(BYTE sc)
{
BYTE mask,ep;

if((SUD[bmRequestType]!=0x02)		// recipient = ENDPOINT
&  (SUD[wValueL]!=0x00))		// wValueL is feature selector, 00 is STALL
	STALL_EP0
else
	{
	mask=rreg(rEPSTALLS);		// read existing bits
	ep = SUD[wIndexL];		// endpoint number
	    if(ep==0x01)
   			{
			ep1stall=sc;
			if (sc==1) mask|=bmEP1OUT; else mask&=~bmEP1OUT;
			wregAS(rEPSTALLS,mask);		// write the bits back and set ACKSTAT
			}
		else if (ep==0x82)
			{
			ep2stall = sc;
			if (sc==1) mask |= bmEP2IN; else mask &= ~bmEP2IN;
			wregAS(rEPSTALLS,mask);		// write the bits back and set ACKSTAT
			}
		else if (ep==0x83)
			{
			ep3stall = sc;
			if (sc==1) mask |= bmEP3IN; else mask &= ~bmEP3IN;
			wregAS(rEPSTALLS,mask);		// write the bits back and set ACKSTAT
			}
		else STALL_EP0
	}
}

void send_descriptor(void) 		// NOTE NOTE This function assumes all packets are 64 or fewer bytes
{
WORD reqlen,sendlen,desclen;		// (but the REQUEST can be larger than 256 bytes, so we need 16-bit length compare)	
BYTE *pDdat;                        // pointer to descriptor table entry
desclen = 0;					// check for zero as error condition (no case statements satisfied)
reqlen = SUD[wLengthL] + 256*SUD[wLengthH];	// 16-bit length can be requested
	switch (SUD[wValueH])			// wValueH is descriptor type
	{
	case	GD_DEVICE:
			{	desclen = DD[0];		// descriptor length
//				pkt.pWrD = DD;
                pDdat = DD;
				break;	}
	case	GD_CONFIGURATION:
			{	desclen = CD[2];		// + 256*CD[3];	config descriptor includes interface, HID, report and ep descriptors
//				pkt.pWrD = CD;
	            pDdat = CD;
				break;	}
	case	GD_STRING:
			{	
				switch	(SUD[2])		// wValueL is string index
				{
				case 0:	
					{	desclen = STR0[0];
                        pDdat = STR0;
						break;	}
				case 1:
					{	desclen = STR1[0];
						pDdat = STR1;
						break;	}
				case 2:
					{	desclen = STR2[0];
                        pDdat = STR2;
						break;	}
				case 3:
					{	desclen = STR3[0];
                        pDdat = STR3; }
				} // end switch
			  break;
			} // end case GD_STRING
	}	// end switch on descriptor type

if (desclen!=0)						// one of the case statements above filled in a value
	{
	sendlen = (reqlen <= desclen) ? reqlen : desclen;	// send the smaller of requested and avaiable (separate var so we can debug)
    writebytes(rEP0FIFO,LSB(sendlen),pDdat);
	wregAS(rEP0BC,sendlen);				// load the EP0BC to arm the EP0-IN transfer ('sendlen' bytes requested) & ACKSTAT
	}
else STALL_EP0
}

void class_request(void)
{}

// Define the following vendor request:
// bRequest: 
// 00	Loop the EP0 data if len!=0
// 01	Transfer mode. Loop EP1-OUT data to EP2-IN (wValueL=2) or EP3-IN (wValueL=3)
// 02	Modify LED. LED value in wValueL (0=OFF, 1=ON) LED number in wIndexL (1-4)
// 03	Read buttons. Return one byte with format b3b2b1b0 where 1=pressed, 0=not
// 04	Read Total Byte Count--copy total_bytes into EP0FIFO, arm for 4 byte transfer
// 05	Clear the total byte counter
	
void vendor_request(void)
{
BYTE dir,len,j,but;
static BYTE vendata[64];			// put the vendor data here
dir=SUD[0]&0x80;					// direction bit tells direction of data packet if requested length is nonzero
//len=SUD[wLengthL]; *** Don't get the length here, get from the byte count register.
len=rreg(rEP0BC);
				
switch(SUD[bRequest])
	{
	case 0:			// 00: Loop the EP0 data
		if ((len!=0)&(dir==0))				// 0=out. Read the EP0 FIFO
			{
			for(j=0; j<len; j++)
				vendata[j]=rreg(rEP0FIFO);
			total_bytes += len;
			Ack_Status();
			}
		else if (len!=0)					// 1=in. Send LEN bytes over EP0.
			{
			for(j=0; j<len; j++)
				wreg(rEP0FIFO,vendata[j]);	// send the data back
			wregAS(rEP0BC,j);				// arm the transfer
			total_bytes += len;
			}
		break;

	case 1: 		// 01: update the xfrmode value
		xfrmode=SUD[wValueL];				// no EP0 data in this transfer
		Ack_Status();
		break;

	case 2:			// 02: update an LED
		switch(SUD[wIndexL])		// which LED?
			{						// no EP0 data, so don't need to clear the EP0FIFO IRQ
			case 1:	if(SUD[wValueL]==1)
		 				L1_ON 
					else 
						L1_OFF 
					total_bytes=0;
					break;
			case 2:	if(SUD[wValueL]==1) L2_ON else L2_OFF break;
			case 3:	if(SUD[wValueL]==1) L3_ON else L3_OFF break;
			case 4:	if(SUD[wValueL]==1) L4_ON else L4_OFF break;
			}
		Ack_Status();
		break;

// Cases 2 and 3 require IN data. Note that IN0BAV will be set going into this routine because the data
// FIFO is always available in a control transfer, having completed its transfer in the previous control transfer.
//		
	case 3:			// 03: Send button value
		but=~rreg(rGPIO);
		but >>= 4;				// shift 4 right
		wreg(rEP0FIFO,but);		// buttons -> EP0FIFO
		wregAS(rEP0BC,1);		// arm the 1 byte transfer
		break;
	case 4:			// 04: send bytes transferred counter
		wreg(rEP0FIFO,(BYTE)total_bytes  &0x000000FF);
		wreg(rEP0FIFO,(BYTE)((total_bytes&0x0000FF00)>>8));
		wreg(rEP0FIFO,(BYTE)((total_bytes&0x00FF0000)>>16));
		wreg(rEP0FIFO,(BYTE)((total_bytes&0xFF000000)>>24));
		wregAS(rEP0BC,4);						// arm IN for 4 bytes
		break;
	case 5:			// clear the byte counter HOMEWORK--figure out why this request hoses all subsequent requests!
		total_bytes=0;
		Ack_Status();
		j=23;		// debug
		break;
	default:	STALL_EP0
	} // switch
} // fn

