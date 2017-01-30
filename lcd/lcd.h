/******************************************************************************

                          eXtreme Electronics xAPI(TM)
						  ----------------------------
xAPI is a Powerful but easy to use C library to program the xBoard(TM)
series of AVR development board. The library has support for commonly use tasks
like:-

*LCD interfacing
*MUXED 7 segment displays.
*Remote Control
*Serial Communication
*DC Motor Controls with Speed Control
*Analog Measurement for Sensor Interface.
*Temperature Measurement.
*I2C Communication.
*EEPROM Interface
*Real Time Clock (RTC Interface)

The APIs are highly documented and easy to use even by a beginner.

For More Info Log On to 
www.eXtremeElectronics.co.in

Copyright 2008-2009 eXtreme Electronics India

                                    LCD Core
						           ----------
This module is used for interfacing with Standard Alpha Numeric LCD Modules.
For More information please see supplied tutorials and videos.

                                     NOTICE
									--------
NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN 
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com

*******************************************************************************/


#include <avr/io.h>

#include <util/delay.h>

#include "myutils.h"

#ifndef _LCD_H
#define _LCD_H
/*_________________________________________________________________________________________*/

/************************************************
	LCD CONNECTIONS
*************************************************/

#define LCD_DATA A			//Port PB0-PB3 are connected to D4-D7
#define LCD_DATA_POS 3	

#define LCD_E A 			//Enable/strobe signal
#define LCD_E_POS	PA2		//Position of enable in above port

#define LCD_RS A			//RS SIGNAL
#define LCD_RS_POS PA0

#define LCD_RW A			//RW SIGNAL
#define LCD_RW_POS PA1

	


//************************************************

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010



/***************************************************
			F U N C T I O N S
****************************************************/

void vLcdByte(uint8_t,uint8_t);
void vLcdBusyLoop(void);
void vLcdInit(uint8_t style);
void vLcdWriteString(const char *msg);
void vLcdWriteInt(int val,unsigned int field_length);
void vLcdGotoXY(uint8_t x,uint8_t y);
void vLcdWriteTime(void);
void vLcdWriteDate(void);
//Low level

#define vLcdCmd(c) (vLcdByte(c,0))
#define vLcdData(d) (vLcdByte(d,1))


/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define vLcdClear() vLcdCmd(0b00000001)
#define vLcdHome() 	vLcdCmd(0b00000010);

#define vLcdWriteStringXY(x,y,msg) {\
		vLcdGotoXY(x,y);\
		vLcdWriteString(msg);\
		}

#define vLcdWriteIntXY(x,y,val,fl) {\
 		vLcdGotoXY(x,y);\
 		vLcdWriteInt(val,fl);\
		}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif




