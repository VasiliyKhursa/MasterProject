/* ***********************************************************************
**
**  Copyright (C) 2013  Vasilij Khursa
**
**  Siemens S65 
**
*********************************************************************** */
// disp.h
#ifndef LS020_H
#define LS020_H


// my includes
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>

#define 	LS020_DDR_RS	DDRB
#define 	LS020_PORT_RS	PORTB
#define 	LS020_RS 		PB7					// сигнал Команды/Данные

#define		LS020_DDR_RST	DDRB
#define 	LS020_PORT_RST	PORTB				// сигнал Ресет	
#define 	LS020_RST 		PB6

#define 	LS020_DDR_CS	DDRB
#define 	LS020_PORT_CS	PORTB				// сигнал Выбор Устройства
#define 	LS020_CS 		PB5

#define 	LS020_Transmir_port 		SPDR


#define Include8x14Font		1	// 0 - не подключать Font8x14.c и функции для Font8x14.c, 	1 - подключать
#define Include6x8Font		1	// 0 - не подключать Font6x8.c и функции для Font6x8.c, 	1 - подключать

/*
#define RS 				PB0		// сигнал Команды/Данные
#define RST 			PB1		// сигнал Ресет	
#define CS_SEL_PIN 		PB2		// сигнал Выбор Устройства
#define CS_NUMBER 		1		// номер сигнал Выбор Устройства для дешифратора
#define AN_CS_NUMBER 	2		// номер сигнал НЕ Выбора Устройства для дешифратора
#define AN_CS_NUMBER 	2		// номер сигнал НЕ Выбора Устройства для дешифратора

#define ls020_TRANSMIT_DATA		PORTB &= ~(1<<RS)		// Clear RS
#define ls020_TRANSMIT_CMD		PORTB |= (1<<RS)  		// Set RS

#define ls020_RESET			  	PORTB &= ~(1<<RST) 		// Pull RST pin down
#define ls020_AN_RESET			PORTB |= (1<<RST) 		// Release RST pin

*/

//use following macros if Decoder is used for Chip Select of ls020
//#include "E:\Disc D\\Sam\Study\Library AVR\My\Proj for Libriraries\Decoder\Decoder.h"
//#define ls020_CS_ASSERT     	CS_Decoder(CS_NUMBER )
//#define ls020_CS_DEASSERT   	CS_Decoder(AN_CS_NUMBER)




//------------------------------------------------------------------------------------------------------
#define ASX 0x1200
#define ASY 0x1300
#define APX 0x1400
#define AEX 0x1500
#define AEY 0x1600
#define APY 0x1700

extern 	uint16_t textcolor;  			// color of border around text
extern 	uint16_t backcolor;    			// color of background
/*
uint16_t textcolor;  	// color of border around text
uint16_t backcolor;    	// color of background
*/

#define bgColor 	0xFFFF
#define fgColor 	0x0000

#define sky 	0x54FB
#define black 	0x0000
#define white 	0xFFFF
#define green 	0x07E0
#define blue 	0x001F
#define red 	0xF800
#define yellow 	0xDEE0
#define orange 	0xAB00
#define pink 	0xF97F
#define brown 	0x8200
#define grey 	0x8410
#define violet 	0x91D4

#define DISP_W 132
#define DISP_H 176
#define CHAR_H 14
#define CHAR_W 8
#define TEXT_COL 16
#define TEXT_ROW 12

/////////////////////////////// function prototypes ////////////////////////

void vLs020WriteCmd8(char cmd8);
void vLs020WriteCmd16(uint16_t cmd16);
void vLs020WriteData8(char data8);
void vLs020WriteData16(uint16_t data16);
void vLs020Init(void);
void vLs020Mode8bit(char BGR);
void vLs020Mode16bit(void);

void vLs020SetDirection(char Direction);

void vLs020SetWindow(char x1, char y1, char x2,char y2, char Direction);
void vLs020PutDot(char x, char y, int color,char bits_color);
void vLs020FillScreen(int color, char bits_color);
void vLs020LineVertical(char y1, char y2, char x, int color, char bits_color);
void vLs020LineHorizontal(char x1, char x2, char y, int color, char bits_color);


void vLs020DrawLine(char x1, char y1, char x2, char y2, int color, char bits_color);
void vLs020DrawCircle(char x0, char y0, char radius, int color, char bits_color);
void vLs020DrawCircleFull(char x0, char y0, char radius, int color, char bits_color);  
void vLs020DrawCircleFull2(char x0, char y0, char radius, int color, char bits_color);void drawCircleFull2(char x0, char y0, char radius, int color, char bits_color);


void vLs020Rectangle(char x1, char y1, char x2, char y2, int color, char bits_color);
void vLs020DrawSquare(char x1, char y1, char x2, char y2, int color, char bits_color);


#if (Include8x14Font == 1)
	void vLs020PutChar8x14(char x, char y, char symbol, int color, int bkcolor, char bits_color, char rot);
	void vLs020PutString8x14(char x, char y, char * text, int color, int bkcolor, char bits_color, char rot);
#endif

#if (Include6x8Font == 1)
	void vLs020PutChar6x8(char x, char y, char symbol, int color, int bkcolor, char bits_color, char rot);
	void vLs020PutString6x8(char x, char y, char * text, int color, int bkcolor, char bits_color, char rot);
#endif


void vLs020Picture();

void vLs020Rotation(char x, char y, char CHARH, char CHARW, char mode);
void vLs020Rotation_2(char x, char y, char CHARH, char CHARW, char mode);
void vLs020ZeroRotation(void);

//--------------------------------После 01.01.2013-------------------------------------------
void vLs020StatusBar(uint8_t start_pos_x,uint8_t start_pos_y,uint8_t width,uint8_t high, int color_border, int color_fon, int color_text, uint8_t bits_color, uint8_t rot, uint8_t old_status, uint8_t new_status);








#endif
// end of file disp.h
