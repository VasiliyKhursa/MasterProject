/* ***********************************************************************
**
**  Copyright (C) 2005  Christian Kranz
**
**  Siemens S65 Display Control
**
*********************************************************************** */
// disp.h
#ifndef LS020_H
#define LS020_H

// my includes
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


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

extern 	uint16_t textcolor;  			// color of border around text
extern 	uint16_t backcolor;    			// color of background

#define 	sky 		0x54FB
#define 	black 		0x0000
#define 	white 		0xFFFF
#define 	green 		0x07E0
#define 	blue 		0x001F
#define 	red 		0xF800
#define 	yellow 		0xDEE0
#define 	orange 		0xAB00
#define 	pink 		0xF97F
#define 	brown 		0x8200
#define 	grey 		0x8410
#define 	violet 		0x91D4

#define 	DISP_W 		132
#define 	DISP_H 		176
#define 	CHAR_H 		14
#define 	CHAR_W 		8
#define 	TEXT_COL 	16
#define 	TEXT_ROW 	12

//------------------------------------------------------------------------------------------------------
#define 	ASX 	0x1200
#define 	ASY 	0x1300
#define 	APX 	0x1400
#define 	AEX 	0x1500
#define 	AEY 	0x1600
#define 	APY 	0x1700



/////////////////////////////// function prototypes ////////////////////////


void fill_screen(int color, char bits_color);
void put_char8x14(char x, char y, char symbol, int color, int bkcolor, char bits_color, char rot);
void put_dot(int x, int y, int color,char bits_color);
void lcd_picture();

void line_vertical(char y1, char y2, char x, int color, char bits_color);
void line_horizontal(char x1, char x2, char y, int color, char bits_color);

void ls020_wrcmd8(char cmd8);
void ls020_wrcmd16(uint16_t cmd16);

void ls020_wrdata8(char data8);
void ls020_wrdata16(uint16_t data16);


void Set_Direction(char Direction);
void ls020_8bit_mode(char BGR);
void ls020_16bit_mode(void);
void ls020_init_lcd(void);
void ls020_set_window(char x1, char y1, char x2,char y2, char Direction);
void ls020_rectangle(char x1, char y1, char x2, char y2, int color, char bits_color);
void ls020_put_char6x8(char x, char y, char symbol, int color, int bkcolor, char bits_color);
void ls020_put_string6x8(char x, char y, char * text, int color, int bkcolor, char bits_color);

void drawSquare(char x1, char y1, char x2, char y2, int color, char bits_color);
void drawLine(int x1, int y1, int x2, int y2, int color, char bits_color);
void drawCircle(int x0, int y0, int radius, int color, char bits_color);
void drawCircleFull(int x0, int y0, int radius, int color, char bits_color); 

void rotationl(char x, char y, char CHARH, char CHARW, char mode);
void rotation(char x, char y, char CHARH, char CHARW, char mode);

#endif
// end of file disp.h
