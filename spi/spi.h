/******************************************************************************
                        
Simple SPI Communication Functions.

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

#ifndef SPI_H
#define SPI_H

//SPI PIN CONFIGURATIONS
#ifdef __AVR_ATmega32__
	#define SPI_PORT 	PORTB
	#define SPI_DDR 	DDRB

	//PINS
	#define SCK_POS 	PB7
	#define MISO_POS 	PB6
	#define MOSI_POS	PB5
	#define SS_POS		PB4
#endif

#ifdef __AVR_ATmega8__
	#define SPI_PORT 	PORTB
	#define SPI_DDR 	DDRB

	//PINS
	#define SCK_POS 	PB5
	#define MISO_POS 	PB4
	#define MOSI_POS	PB3
	#define SS_POS		PB2
#endif

#ifdef __AVR_ATmega2560__
	#define SPI_PORT 	PORTB
	#define SPI_DDR 	DDRB

	//PINS
	#define SCK_POS 	PB1
	#define MISO_POS 	PB3
	#define MOSI_POS	PB2
	#define SS_POS		PB0
#endif

//CS : Chip Select
#define CS_PORT PORTB
#define CS_DDR	DDRB
#define CS_POS PB0

#define CS_HIGH() CS_PORT|=(1<<CS_POS)
#define CS_LOW() CS_PORT&=(~(1<<CS_POS))

void SPIInit();
void SPIClose();
uint8_t SPIWrite(uint8_t data);

#endif
