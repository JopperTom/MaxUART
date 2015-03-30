// SPI6.c	6-16-04	LTH 
// HID that sends a constant text stream to the active Windows window. 
// Checks PB5 (8051 INT5#) for active low level interrupt requests.
//
#include "SPI.h"
#include "HIDKB_enum_data.h"	// HID keyboard enumeration data (same as transceiver tester)
#define TIMEOUT 8000			// time between PB0 button checks (send/inhibit sending KB chars)
//Global variables
SPI_PKT pkt;			// SPI packet structure
BYTE SUD[8];			// my copy of setup data
BYTE msgidx,msglen;		// message index and length
BYTE configval;			// From Set_Configuration, reported back in Get_Configuration
BYTE ep3stall;			// flag for EP3 Stall, set by Set_Feature, reported back in Get_Status
BYTE interfacenum;
BYTE inhibit_send;		// flag for the keyboard character send routine
BYTE RWU_enabled;
BYTE Suspended;
//
// prototypes
void Initialize(void);
void do_SETUP(void);
void do_IN3(void);

void Initialize(void)		// call at power-on or USB Bus Reset
{
// Enable application-required interrupts here since they may be cleared by a USB bus reset
//
wreg(rEPIEN,(bmSUDAV | bmIN3BAV));		// Enable the SUDAV and IN3 interrupts
//wreg(rUSBIEN,(bmUSBRES|bmUSBRESDN|bmSUSPEND)); 
wreg(rUSBIEN,(bmUSBRES|bmUSBRESDN));	// NOTE: SUSPEND interrupt enabled when the device is configured
configval=0;							// at pwr on OR bus reset we're unconfigured
write_leds(0xFF);						// blank the display
}

// ****************************** MAIN ********************************
void main(void)
{
BYTE itest1,itest2,stat;		// for polling the interrupt requests
BYTE epirq,usbirq,bmap,mask;	// for testing the SPI read byte
WORD button_time,bs;			// time delay for polling the pushbuttons, button states
ep3stall=0;					// EP3 inintially un-halted (no stall) (CH9 testing)
msgidx = 0;					// start of KB Message[]
msglen = sizeof(Message);
button_time = 0;
inhibit_send = 0x01;		// 00 means send, 01 means inhibit sending
//
SPI_Init();					// Set SPI bus to "idle" (SCK low, CS# high)
Reset_RT86(1000);
Initialize();				// initialze stuff here that should not be re-initialized during USB reset
Suspended=0;
RWU_enabled=0;
// EP3-IN. Load "key up" code (00 00 00) to initialize for HID
wreg(rEP3INFIFO,0);			
wreg(rEP3INFIFO,0);						
wreg(rEP3INFIFO,0);					
wreg(rEP3INBC,3);			// arm the first EP3-IN transfer


// PINCTL, USBCTL and GPIO regs are not reset by USB bus reset, so they can be set here (not in Initialize())
// For testsing the three interrupt pin modes:
#ifdef NEG_LEVEL_INT
wreg(rPINCTL,(bmFDUPSPI|bmINTLEVEL));	// INTLEVEL=1 (level), POSINT=X in level mode (always neg)
#endif
#ifdef POS_PULSE_INT 
wreg(rPINCTL,(bmFDUPSPI|bmPOSINT));	// INTLEVEL=0 (pulse), POSINT=1 (posedge edge)
#endif
#ifdef NEG_PULSE_INT
wreg(rPINCTL,bmFDUPSPI);			// INTLEVEL=0 (pulse), POSINT=0 (negative edge)
#endif

wreg(rGPIO,0xFF);					// lites off
wreg(rUSBCTL,bmCONNECT);				// since VBGATE=0, CONNECT is not conditional on Vbus present
wreg(rCPUCTL,bmIE);						// Enable interrupt pin
//
while(1)					// endless loop
	{
		do	
		{
			button_time++;
			if (button_time == TIMEOUT)
				{
				button_time = 0;
				bs = ~rreg(rGPIO);				// bs=button states. complement to make the buttons active high
				if(Suspended)					// check the remote wakeup button only if suspended
					{
						if (bs&0x80)				// remote wakeup button
							{
							SETBIT(rUSBCTL,bmRESUME)	// signal resume
							L3_ON						// turn on signaling-resume light [yel]
//						
							while ((rreg(rUSBIRQ)&bmRSUMDN)==0) ;	// spin until resume signaling done
							CLRBIT(rUSBCTL,bmRESUME)	// remove the RESUME signal 
							wreg(rUSBIRQ,bmRSUMDN);		// clear the IRQ
							Suspended=0;				// stop checking the button
							L2_OFF				// turn off suspend light [red]
							L3_OFF				// turn off signaling-resume light [yel]
							}
			   		}

	   			if (bs&0x10)					// check the "send" pushbutton
					{
					inhibit_send = 0x00;
					L1_ON
					}
				else if (bs&0x20)				// check the "stop sending" pushbutton
					{
					inhibit_send = 0x01;
					L1_OFF
					}
				} // if (button_time == TIMEOUT)
		} // do 
		while (Check_INT() == 0);			// while RT86 pin is high, just check the buttons
/*
		stat 	= readstat();				// Check the SPI status byte against individual register bits
		epirq 	= rreg(rEPIRQ);
		usbirq	= rreg(rUSBIRQ);
		mask	= (usbirq << 3) & 0xC0;		// shift SUSPEND and USBRES bits to b7b6 and mask them
		bmap	= mask | epirq;				// should equal the readstat bits	
		if(bmap != stat) 
 			while(1);
*/
		itest1 = rreg(rEPIEN) & rreg(rEPIRQ);	// only consider the enabled ones
		itest2 = rreg(rUSBIEN) & rreg(rUSBIRQ);	// ditto
		
// Something is pending. The order of the if statements below sets the "priority".
	if(itest1 & bmSUDAV) 
		{
		wreg(rEPIRQ,bmSUDAV);			// clear the SUDAV IRQ
		write_leds(0);					// write "0" to the display
		do_SETUP();
		}
	else if(itest1 & bmIN3BAV) 
		{
		wreg(rEPIRQ,bmIN3BAV);		// clear the IRQ
		do_IN3();
		}
// SUSPEND-RESUME Section
	else if(itest2 & bmSUSPEND)			// HOST suspended bus for 3 msec
		{
		wreg(rUSBIRQ,bmSUSPEND);		// clear the IRQ (needed?--it's level active)
		CLRBIT(rUSBIEN,bmSUSPEND);		// de-activate the SUSPEND IRQ so we don't keep getting it during suspend

		L2_ON							// turn on the SUSPEND light [red]
		wreg(rUSBIRQ,bmBUSACT);			// clear remnants of bus activity
		SETBIT(rUSBIEN,bmBUSACT);		// enable 'resume' interrupt
		Suspended=1;					// so main loop can check for a button press
		}
	else if(itest2 & bmBUSACT)			// use this for HOST RESUME indication		
		{
		wreg(rUSBIRQ,bmBUSACT);			// clear the IRQ
		CLRBIT(rUSBIEN,bmBUSACT);		// disable 'resume' interrupt
		SETBIT(rUSBIEN,bmSUSPEND);		// enable SUSPEND interrupt
		L2_OFF
		Suspended=0;					// flag to stop checking the RESUME button
		}
	else if(itest2 & bmRSUMDN)		
		{
		wreg(rUSBIRQ,bmRSUMDN);	
		}
	else if(itest1 & bmIN2BAV) 
		{
		debug(2);		
		wreg(rEPIRQ,bmIN2BAV);		// JUST clear the IRQ
		}
	else if(itest1 & bmOUT1DAV)		
		{
		debug(1);		
		wreg(rEPIRQ,bmOUT1DAV); 
		}
	else if(itest1 & bmOUT0DAV)		
		{
		debug(6);		
		wreg(rEPIRQ,bmOUT0DAV); 
		}
	else if(itest1 & bmIN0BAV)		
		{
		wreg(rEPIRQ,bmIN0BAV);	
		}
//
	else if(itest2 & bmNOVBUS)		
		{
		debug(8);		
		wreg(rUSBIRQ,bmNOVBUS); 
		}

// USB Bus Reset Section		
	else if(itest2 & bmUSBRES)		
		{
		L4_ON							// green LED
		wreg(rUSBIRQ,bmUSBRES);			// clear the IRQ
		}
	else if(itest2 & bmUSBRESDN)		
		{
		wreg(rUSBIRQ,bmUSBRESDN);
		Initialize();					// start over
		L4_OFF							// green LED
		}
	else if(itest2 & bmOSCOK)		
		{
		debug(0x0D);		
		wreg(rUSBIRQ,bmOSCOK);
		}
	} // end while(1)
} // end main

void do_SETUP(void)
{							// got a SETUP packet. Clear the IRQ and read 8 SETUP bytes
// Read the 8 SETUP bytes in a burst (one CS interval for 8 read bytes)
pkt.CMD		=	0x20;		// Command byte = read R4
pkt.WrLen	=	0;			// write 0 bytes
pkt.RdLen 	=	8;			// read 8 bytes--SETUP data is in R4
pkt.pRdD	= 	SUD;		// put data into SUD[8]
SPI_wtr(&pkt);			
							// Parse the SETUP packet
switch(SUD[bmRequestType]&0x60)		// look only at b6&b5
	{
	case 0x00:	std_request();		break;
	case 0x20:	class_request();	break;
	case 0x40:	vendor_request();	break;
	default:	STALL_EP0
	}
}
// *******************************************************************************************
void do_IN3(void)
{
if (inhibit_send==0x01)
	{
	wreg(rEP3INFIFO,0);			// load the "key up" code
	wreg(rEP3INFIFO,0);
	wreg(rEP3INFIFO,0);
	}
else
	{
	wreg(rEP3INFIFO,Message[msgidx++]);	// load the next keystroke (3 bytes)
	wreg(rEP3INFIFO,Message[msgidx++]);
	wreg(rEP3INFIFO,Message[msgidx++]);
	if(msgidx >= msglen) msgidx=0;		// check for message wrap
	}
wreg(rEP3INBC,3);						// arm it
}

// ********************************************************************************************************
void std_request(void)
{
BYTE dum;					// dummy byte to read register with AckStat bit set
switch(SUD[bRequest])			
	{
	case	SR_GET_DESCRIPTOR:		send_descriptor();		break;
	case	SR_SET_FEATURE:			feature(1);				break;
	case	SR_CLEAR_FEATURE:		feature(0);				break;
	case	SR_GET_STATUS:			get_status();			break;
	case	SR_SET_INTERFACE:		set_interface();		break;
	case	SR_GET_INTERFACE:		get_interface();		break;
	case	SR_GET_CONFIGURATION: 	get_configuration();	break;
	case	SR_SET_CONFIGURATION: 	set_configuration();	break;
	case	SR_SET_ADDRESS:			
				dum=rregAS(rFNADDR);						break;
	default:STALL_EP0
	}
}

void set_configuration(void)
{
BYTE a,b,c,dumval;
configval=SUD[wValueL];				// Config value is here
	if (configval != 0)
		SETBIT(rUSBIEN,bmSUSPEND);	// enable the suspend interrupt
dumval=rregAS(rFNADDR);				// dummy read to set the ACKSTAT bit
}

void get_configuration(void)
{
wreg(rEP0FIFO,configval);
wregAS(rEP0BC,1);
}

// *******************************************************************************************
void set_interface(void)		// All we accept are Interface=0 and AlternateSetting=0, otherwise send STALL
{
BYTE dumval;
if((SUD[wValueL]==0)		// wValueL=Alternate Setting index
  &(SUD[wIndexL]==0))		// wIndexL=Interface index
  	dumval=rregAS(rFNADDR);		// dummy read to set the ACKSTAT bit
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
BYTE testbyte;
testbyte=SUD[bmRequestType];
switch(testbyte)	
	{
	case 0x80: 					// directed to DEVICE
		wreg(rEP0FIFO,RWU_enabled);		// first byte is 000000rs where r=enabled for RWU and s=self-powered.
		wreg(rEP0FIFO,0x00);			// second byte is always 0
		wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
		break; 				
	case 0x81: 					// directed to INTERFACE
		wreg(rEP0FIFO,0x00);			// this one is easy--two zero bytes
		wreg(rEP0FIFO,0x00);		
		wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
		break; 				
	case 0x82: 					// directed to ENDPOINT
		if(SUD[wIndexL]==0x83)			// We only reported ep3, so it's the only one the host can stall IN3=83
			{
			wreg(rEP0FIFO,ep3stall);		// first byte is 0000000h where h is the halt (stall) bit
			wreg(rEP0FIFO,0x00);			// second byte is always 0
			wregAS(rEP0BC,2); 				// load byte count, arm the IN transfer, ACK the status stage of the CTL transfer
			break;
			}
		else	STALL_EP0		// Host tried to stall an invalid endpoint (not 3)				
	default:	STALL_EP0		// the host screwed up
	}
}

// **********************************************************************************************
// FUNCTION: Set/Get_Feature. Call as feature(1) for Set_Feature or feature(0) for Clear_Feature.
// There are two set/clear feature requests:
//		To a DEVICE: 	Remote Wakeup (RWU). When RWU is enabled, enable the SUSPEND interrupt.
//						(Otherwise, SUSPEND will trigger if not plugged in).
//  	To an ENDPOINT:	stall (EP3 only for this app)
//
void feature(BYTE sc)
{
BYTE mask;
	if((SUD[bmRequestType]==0x02)	// dir=h->p, recipient = ENDPOINT
	&  (SUD[wValueL]==0x00)			// wValueL is feature selector, 00 is EP Halt
	&  (SUD[wIndexL]==0x83))		// wIndexL is endpoint number IN3=83
			{
			mask=rreg(rEPSTALLS);		// read existing bits
				if(sc==1)				// set_feature
					{
					mask += bmEP2IN;	// set only this bit
					ep3stall=1;
					}
				else					// clear_feature
					{
					mask &= !bmEP2IN;	// clear only this bit
					ep3stall=0;
					}
			wregAS(rEPSTALLS,mask);		// write the bits back and set ACKSTAT
			}
	else if ((SUD[bmRequestType]==0x00)	// dir=h->p, recipient = DEVICE
		  &  (SUD[wValueL]==0x01))		// wValueL is feature selector, 01 is Device_Remote_Wakeup
			{
			RWU_enabled = sc<<1;		// =2 for set, =0 for clear feature. The shift puts it in the get_status bit position.			
			rregAS(rFNADDR);			// dummy read to set ACKSTAT
			}
	else STALL_EP0
}

void send_descriptor(void)
{
WORD reqlen,sendlen,desclen;
// NOTE NOTE This function assumes all packets are 64 or fewer bytes
desclen = 0;							// check for zero as error condition (no case statements satisfied)
reqlen = SUD[wLengthL] + 256*SUD[wLengthH];	// 16-bit
	switch (SUD[wValueH])				// wValueH is descriptor type
	{
	case	GD_DEVICE:
			{	desclen = DD[0];		// descriptor length
				pkt.pWrD = DD;
				break;	}
	case	GD_CONFIGURATION:
			{	desclen = CD[2];		// + 256*CD[3];	config descriptor includes interface, HID, report and ep descriptors
				pkt.pWrD = CD;
				break;	}
	case	GD_STRING:
			{	
				switch	(SUD[2])		// wValueL is string index
				{
				case 0:	
					{	desclen = STR0[0];
						pkt.pWrD = STR0;
						break;	}
				case 1:
					{	desclen = STR1[0];
						pkt.pWrD = STR1;
						break;	}
				case 2:
					{	desclen = STR2[0];
						pkt.pWrD = STR2;
						break;	}
				case 3:
					{	desclen = STR3[0];
						pkt.pWrD = STR3; }
				} // end switch
			  break;
			} // end case GD_STRING
				
	case	GD_HID:
			{	desclen = HD[0];
			pkt.pWrD = HD;
			break;	}
	case	GD_REPORT:
			{	desclen = HD[7];		// + 256*HD[8];
				pkt.pWrD = RepD;
				break; }
	}	// end switch on descriptor type

if (desclen!=0)							// one of the case statements above filled in a value
	{
	pkt.CMD = 0x02;						// Command = Write R0
	sendlen = (reqlen <= desclen) ? reqlen : desclen;	// send the smaller of requested and avaiable (separate var so we can debug)
	pkt.WrLen	=  LSB(sendlen);		// n byte packet
	pkt.RdLen	=	0;
	SPI_wtr(&pkt);						// Load the requested descriptor into EP0FIFO
	wregAS(rEP0BC,sendlen);					// load the EP0BC to arm the EP0-IN transfer ('sendlen' bytes requested) & ACKSTAT
	}
else STALL_EP0
// if(dum2==1) while(1){};	// debug HANG
}

void class_request(void)
{
switch(SUD[bRequest])
	{
	case GET_REPORT:
		if (SUD[wValueH]==INPUT_REPORT)
			send_keystroke(0);					// send a keystroke over EP0-IN
		break;
//	case GET_IDLE:		STALL_EP0 break;
//	case GET_PROTOCOL:	STALL_EP0 break;
//	case SET_REPORT:	STALL_EP0 break;
//	case SET_IDLE:		SETBIT(rEPSTALLS,bmCTLSTAT); break;	
//	case SET_PROTOCOL:	STALL_EP0 break;
	default:	STALL_EP0
	}
}

// send_keystroke is called for two cases: GET_REPORT(INPUT) over EP0 or an IN sent to the interrupt pipe EP3.	
void send_keystroke(BYTE ep)
{
wreg(ep,Message[msgidx]);
msgidx++;
wreg(ep,Message[msgidx]);
msgidx++;
if (msgidx==msglen)	msgidx=0;	// wrap around the message index
if (ep==0)	wregAS(rEP0BC,3);	// Arm EP0 and ACKSTAT
else		wreg(rEP3INBC,3);	// Arm EP3
}

void vendor_request(void)
{}


