/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#include "D:\Sam\Study\Library AVR\My\Proj for Libriraries\SPI\spi.h"
#include "D:\Sam\Study\Library AVR\My\Proj for Libriraries\ls020\disp.h"




int dots = 0x00;


char str1[1] = "0";
char str2[1] = ".";
char str3[1] = "0";
char str4[1] = "0";
char str5[1] = "0";
char str6[1] = "0";

char ADC1 = 0;
char ADC2 = 0;
char ADC3 = 0;
char ADC4 = 0;
char ADC5 = 0;


char ADC1B = 0;
char ADC2B = 0;
char ADC3B = 0;
char ADC4B = 0;
char ADC5B = 0;

char first = 0;

int ADCB = 0;


int main4444 (void)
{

	InitSPI(0,0,0,0);
	ls020_init_lcd(); 						// Init LS020 LCD
	ls020_8bit_mode(0); 					// Set 8bit LCD host mode
	ls020_rectangle(0,0,175,131,bgColor,8);



//	ls020_status_bar(10, 10, 102, 12, black, green, black, 16, 1, 0, 99);


	for (char i = 0; i<=100; i++)
		{
		ls020_status_bar(10, 10, 120, 12, black, green, black, 16, 1, i-1, i);
		_delay_ms(100);
		}


	
	
	while(1)
	{

	}
	return 1;

	
}

*/
