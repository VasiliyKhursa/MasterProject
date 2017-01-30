/******************************************************************************
                        
Sample Program to Test "External SPI ADC Interface" libraries. The Simple job 
performed by this program is as follows:-

*to connect with external SPI ADC
*ask it to convert a analog value to digital on any channel say channel 0
*read the converted value
*display it in LCD Module



Hardware:
	ATmega32 running @ 16MHz
	FUSE HIGH = 0xC9 LOW=0xFF

	MCP320X Connected to SPI Port

	CS PIN of MCP320X Connected to PD7 (Can be chaned by editing spi.h)

	16x2 LCD Module Connected as
		* RS 	-> PD3
		* R/W 	-> PD6
		* E		-> PB4
		* D4	-> PB0
		* D5	-> PB1
		* D6	-> PB2
		* D7	-> PB3


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
#include "lcd.h"
#include "spi.h"
#include "adc_ex.h"

void main()
{

	//Initialize LCD Module
	LCDInit(LS_ULINE);
	
	//Initialize External ADC Module
	InitADCEx();
	
	//A varriable to hold the converted value.
	uint16_t result;
		
		//Read Channel Number 0
		result=ReadADCEx(0);

		//Display
		LCDWriteStringXY(0,0,"MCP3208 ADC Test");
		LCDWriteStringXY(0,1,"result = ");

		LCDWriteIntXY(9,1,result,5);

	while(1)
	{
		


//		_delay_ms(1000);
	}

}
	
		
