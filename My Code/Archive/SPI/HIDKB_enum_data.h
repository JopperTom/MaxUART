// USBdata.h
// Enumeration tables, keyboard data.

BYTE DD[]=			// DEVICE Descriptor
    {0x12,	       		// bLength = 18d
    0x01,				// bDescriptorType = Device (1)
	0x00,0x01,			// bcdUSB(L/H) USB spec rev (BCD)
	0xFF,0xFF,0xFF, 	// bDeviceClass, bDeviceSubClass, bDeviceProtocol
	0x40,				// bMaxPacketSize0 EP0 is 64 bytes
	0x6A,0x0B,			// idVendor(L/H)--Maxim is 0B6A
	0x46,0x53,			// idProduct(L/H)--5346
	0x34,0x12,			// bcdDevice--1234
	1,2,3,				// iManufacturer, iProduct, iSerialNumber
	1};					// bNumConfigurations

BYTE CD[]=			// CONFIGURATION Descriptor
	{0x09,				// bLength
	0x02,				// bDescriptorType = Config
	0x22,0x00,			// wTotalLength(L/H) = 34 bytes
	0x01,				// bNumInterfaces
	0x01,				// bConfigValue
	0x00,				// iConfiguration
	0xE0,				// bmAttributes. b7=1 b6=self-powered b5=RWU supported
	0x01,				// MaxPower is 2 ma
// INTERFACE Descriptor
	0x09,				// length = 9
	0x04,				// type = IF
	0x00,				// IF #0
	0x00,				// bAlternate Setting
	0x01,				// bNum Endpoints
	0x03,				// bInterfaceClass = HID
	0x00,0x00,			// bInterfaceSubClass, bInterfaceProtocol
	0x00,				// iInterface
// HID Descriptor
	0x09,				// bLength
	0x21,				// bDescriptorType = HID
	0x10,0x01,			// bcdHID(L/H) Rev 1.1
	0x00,				// bCountryCode (none)
	0x01,				// bNumDescriptors (one report descriptor)
	0x22,				// bDescriptorType	(report)
	0x2B,0x00,			// wDescriptorLength(L/H) (report descriptor size is 43 bytes)
// Endpoint Descriptor
	0x07,				// bLength
	0x05,				// bDescriptorType (Endpoint)
	0x83,				// bEndpointAddress (EP3-IN)		// this was EP1 in transceiver tester code
	0x03,				// bmAttributes	(interrupt)
	0x40,0x00,			// wMaxPacketSize (64)
	0xFF,				// bInterval (poll every 255 msec)
// Report Descriptor
	0x05,				// bDescriptorType (report)
	0x01,				// Usage Page (generic desktop)
	0x09,0x06,			// Usage
	0xA1,0x01,			// Collection
	0x05,0x07,			//   Usage Page
	0x19,0xE0,			//	 Usage Minimum = 224
	0x29,0xE7,			//   Usage Maximum = 231
	0x15,0x00,			//   Logical Minimum = 0
	0x25,0x01,			//   Logical Maximum = 1
	0x75,0x01,			// 	 Report Size = 1
	0x95,0x08,			//   Report Count = 8
	0x81,0x02,			//  Input(Data,Variable,Absolute)
	0x95,0x01,			//	 Report Count = 1
	0x75,0x08,			//   Report Size = 8
	0x81,0x01,			//  Input(Constant)
	0x19,0x00,			//   Usage Minimum = 0
	0x29,0x65,			//   Usage Maximum = 101
	0x15,0x00,			//   Logical Minimum = 0,
	0x25,0x65,			//   Logical Maximum = 101
	0x75,0x08,			//   Report Size = 8
	0x95,0x01,			//   Report Count = 1
	0x81,0x00,			//  Input(Data,Variable,Array)
	0xC0};				// End Collection

// HID Descriptor (again, so we can respond to the GD_HID request)
BYTE HD[]=
	{
	0x09,				// bLength
	0x21,				// bDescriptorType = HID
	0x10,0x01,			// bcdHID(L/H) Rev 1.1
	0x00,				// bCountryCode (none)
	0x01,				// bNumDescriptors (one report descriptor)
	0x22,				// bDescriptorType	(report)
	0x2B,0x00};			// wDescriptorLength(L/H) (report descriptor size is 43 bytes)

// Report descriptor (again so we can do GD_Report) Not efficient, but maximum code portability.
BYTE RepD[]=
	{
	0x05,				// bDescriptorType (report)
	0x01,				// Usage Page (generic desktop)
	0x09,0x06,			// Usage
	0xA1,0x01,			// Collection
	0x05,0x07,			//   Usage Page
	0x19,0xE0,			//	 Usage Minimum = 224
	0x29,0xE7,			//   Usage Maximum = 231
	0x15,0x00,			//   Logical Minimum = 0
	0x25,0x01,			//   Logical Maximum = 1
	0x75,0x01,			// 	 Report Size = 1
	0x95,0x08,			//   Report Count = 8
	0x81,0x02,			//  Input(Data,Variable,Absolute)
	0x95,0x01,			//	 Report Count = 1
	0x75,0x08,			//   Report Size = 8
	0x81,0x01,			//  Input(Constant)
	0x19,0x00,			//   Usage Minimum = 0
	0x29,0x65,			//   Usage Maximum = 101
	0x15,0x00,			//   Logical Minimum = 0,
	0x25,0x65,			//   Logical Maximum = 101
	0x75,0x08,			//   Report Size = 8
	0x95,0x01,			//   Report Count = 1
	0x81,0x00,			//  Input(Data,Variable,Array)
	0xC0};				// End Collection


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
	'R',0,				
	'T',0,
	'8',0,
	'6',0,
	' ',0,
	'L',0,
	'i',0,
	'v',0,
	'e',0,
	's',0,
	'!',0};

BYTE STR3[]=			// Product ID
	{24,				// bLength
	0x03,				// bDescriptorType = string
	'S',0,				
	'/',0,
	'N',0,
	' ',0,
	'1',0,
	'2',0,
	'3',0,
	'4',0,
	'L',0,
	'T',0,
	'H',0};

BYTE Message[]={		// each letter is 3 bytes, shift, 00, keycode
	0x02,0x00,0x10,		// M (02 is shift)
	0x00,0x00,0x04,		// a
	0x00,0x00,0x1B,		// x
	0x00,0x00,0x0C,		// i
	0x00,0x00,0x10,		// m
	0x00,0x00,0x2C,		// (sp)

	0x02,0x00,0x15,		// R
	0x02,0x00,0x17,		// T
	0x00,0x00,0x25,		// 8
	0x00,0x00,0x23,		// 6
	0x00,0x00,0x2C,		// (sp)
	
	0x02,0x00,0x07,		// D
	0x00,0x00,0x12,		// o
	0x00,0x00,0x08,		// e
	0x00,0x00,0x16,		// s
	0x00,0x00,0x2C,		// (sp)
	0x02,0x00,0x18,		// U
	0x02,0x00,0x16,		// S
	0x02,0x00,0x05,		// B
	0x02,0x00,0x1E,		// !
	0x00,0x00,0x2C};	// (sp)

