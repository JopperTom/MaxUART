// USBdata.h
// Enumeration tables, keyboard data.
// Changes from Keil code--'code' changed to 'const'.

BYTE DD[]=			// DEVICE Descriptor
    {18,	       		// bLength = 18d
    0x01,				// bDescriptorType = Device (1)
	0x00,0x02,			// bcdUSB(L/H) USB spec rev (BCD)
	0xFF,0xFF,0xFF, 	// bDeviceClass, bDeviceSubClass, bDeviceProtocol
	64,					// bMaxPacketSize0 EP0 is 64 bytes
	0xD7,0x0B,			// APA vid = BD7
	0x02,0x10,			// APA pid = 1002
//	0x47,0x05,			// idVendor(L/H)--CY is 0547
//	0x31,0x21,			// idProduct(L/H)--1002 is to bind with the GP driver without downloading the monitor
//	0x02,0x10,
	0x04,0x00,			// bcdDevice--0004
	1,2,3,				// iManufacturer, iProduct, iSerialNumber
	1};					// bNumConfigurations

BYTE CD[]=			// CONFIGURATION Descriptor
	{0x09,				// bLength
	0x02,				// bDescriptorType = Config
	39,0,				// wTotalLength(L/H)
	0x01,				// bNumInterfaces
	0x01,				// bConfigValue
	0x00,				// iConfiguration
	0x80,				// bmAttributes. Self-powered, no RWU
	0x01,				// MaxPower is 2 ma
// INTERFACE Descriptor--AS=0
	0x09,				// length = 9
	0x04,				// type = IF
	0x00,				// IF #0
	0x00,				// bAlternate Setting = 0
	0x03,				// bNum Endpoints (not counting EP0)
	0xFF,0xFF,0xFF,		// bInterfaceClass, bInterfaceSubClass, bInterfaceProtocol = Vendor
	0x00,				// iInterface (string index)
// Endpoint Descriptor
	0x07,				// bLength
	0x05,				// bDescriptorType (Endpoint)
	0x01,				// bEndpointAddress (EP1-OUT) (MSB=1 for IN, 0 for OUT)
	0x02,				// bmAttributes	(bulk)
	0x40,0x00,			// wMaxPacketSize (64)
	0x00,				// bInterval (10 frames)
// Endpoint Descriptor
	0x07,				// bLength
	0x05,				// bDescriptorType (Endpoint)
	0x82,				// bEndpointAddress (EP2-IN)		// this was EP1 in transceiver tester code
	0x03,				// bmAttributes	(interrupt)
	0x40,0x00,			// wMaxPacketSize (64)
	0x0A,				// bInterval (10 frames)
// Endpoint Descriptor
	0x07,				// bLength
	0x05,				// bDescriptorType (Endpoint)
	0x83,				// bEndpointAddress (EP3-IN)		// this was EP1 in transceiver tester code
	0x02,				// bmAttributes	(bulk)
	0x40,0x00,			// wMaxPacketSize (64)
	0x00};				// bInterval (0 for BULK)


BYTE STR0[]=		// Language string
	{0x04,				// bLength
	0x03,				// bDescriptorType = string
	0x09,0x04};			// wLANGID(L/H)

BYTE STR1[]=			// Manufacturer ID
	{12,				// bLength
	0x03,				// bDescriptorType = string
	'M',0,				// love that Unicode!
	'a',0,
	'x',0,
	'i',0,
	'm',0};

BYTE STR2[]=			// Product ID
	{24,				// bLength
	0x03,				// bDescriptorType = string
	'B',0,				
	'u',0,
	'l',0,
	'k',0,
	' ',0,
	'L',0,
	'o',0,
	'o',0,
	'p',0,
	'e',0,
	'r',0};

BYTE STR3[]=			// serial number
	{24,				// bLength
	0x03,				// bDescriptorType = string
	'A',0,				
	'P',0,
	'A',0,
	' ',0,
	'v',0,
	'i',0,
	'd',0,
	'&',0,
	'p',0,
	'i',0,
	'd',0};


