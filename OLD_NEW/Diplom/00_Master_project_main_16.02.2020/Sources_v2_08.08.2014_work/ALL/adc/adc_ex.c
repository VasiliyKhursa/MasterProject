/******************************************************************************
                        
Small Library to easily interface AVR MCUs with External SPI ADCs. This library
provides a set of fuctions to work with Microchip Inc's SPI ADC Chips like

	*MCP3204 : 12 BIT, 100ksps, 4 Channels
	*MCP3208 : 12 BIT, 100ksps, 8 Channels

Hardware:
	ATmega32 running @ 16MHz
	FUSE HIGH = 0xC9 LOW=0xFF

	MCP320X Connected to SPI Port

	CS PIN of MCP320X Connected to PD7 (Can be chaned by editing spi.h)



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

#include "spi.h"
#include "adc_ex.h"
#include <util/delay.h>

/********************************************************************

Function To Initialize the External ADC Chip

Arguments:
		NONE

Retuen Value:
		NONE

NOTE:
		Must be called before performing a conversion.
		Call it at program startup.

********************************************************************/
void InitADCEx()
{
	SPIInit();
}



/********************************************************************

Requests the ADC to perform conversion and send the result.

Arguments:
		uint8_t ch : Channel Number
		For MCP3204 ch is between 0-3 (Total 4 channels)
		For MCP3208 ch is between 0-7 (Total 8 channels)

Return Value:(TYPE uint16_t, i.e a 16bit unsigned int)
		The digital equivalent of analog input on selected channel.
		Since the ADCs are 12 bit the return value is between
		0-4095 (Including both)

********************************************************************/
uint16_t ReadADCEx(uint8_t ch)
{
	uint8_t byte,data_high,data_low;

	byte=0b00000110;

	if(ch>3)
		byte|=0b00000001;

	CS_LOW();
//	_delay_ms(2000);
	
	SPIWrite(byte);

	byte=ch<<6;

	data_high=SPIWrite(byte);

	data_high&=0b00001111;

	data_low=SPIWrite(0xFF);

	CS_HIGH();
//	_delay_ms(2000);
	return ((data_high<<8)|data_low);
}
