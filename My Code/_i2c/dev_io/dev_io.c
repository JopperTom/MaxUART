#include <ezusb.h>
#include <ezregs.h>

#define PF_IDLE			0
#define PF_GETKEYS		1

#define KEY_WAKEUP		0
#define KEY_F1			1
#define KEY_F2			2
#define KEY_F3			3

#define BTN_ADDR		0x20
#define LED_ADDR		0x21

BYTE xdata Digit[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x98, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e };

main()
{
	BYTE	num = 0;
	BYTE xdata	buttons;
	BYTE	kstates = 0xff;
	BYTE	kdeltas;
	BYTE	key;
	BYTE	display = TRUE;

	EZUSB_InitI2C();			// Initialize EZ-USB I2C controller

	while(TRUE)
	{
		EZUSB_ReadI2C(BTN_ADDR,0x01,&buttons);	// Read button states

		kdeltas = kstates ^ buttons;			// 
		kstates = buttons;
		key = 0;

		while(kdeltas)
		{
			if(kdeltas & 0x01)
			{
				if(!((kstates >> key) & 0x01))
					switch(key)
					{
						case KEY_F1:
							if(--num > 0x0f)
								num = 0x0f;
							break;
						case KEY_F2:
							if(++num > 0x0f)
								num = 0;
							break;
						case KEY_WAKEUP:
							num = 0;
							break;
						case KEY_F3:
							num = 0x0f;
					}
				display = TRUE;
			}
			kdeltas = kdeltas >> 1;
			++key;
		}
		if(display)
		{
			EZUSB_WriteI2C(LED_ADDR, 0x01, &(Digit[num]));
			EZUSB_WaitForEEPROMWrite(LED_ADDR);
			display = FALSE;
		}
	}
}
