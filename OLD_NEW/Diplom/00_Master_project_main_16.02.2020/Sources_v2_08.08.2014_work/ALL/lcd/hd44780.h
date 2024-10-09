#pragma once

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#define HD44780_PIN		PINA
#define HD44780_PORT	PORTA
#define HD44780_DDR		DDRA

#define HD44780_RS		PA0
#define HD44780_RW		PA1
#define HD44780_E		PA2

#define HD44780_DB		PA3//-PA3

#define HD44780_DB_MASK	(0xf<<HD44780_DB)

#define LCD_CLEAR				0x01
#define LCD_HOME				0x02
#define LCD_MOVEDIR(id,s)		(0x04 | (((id) << 1)) | (s))
#define LCD_MODE(d,c,b)			(0x08 | (((d) << 2)) | ((c) << 1) | (b))
#define LCD_SHIFT(sc,rl)		(0x10 | ( ((sc) << 3) | ((rl) << 2)) )
#define LCD_GOTO_CGRAM(pos)		(0x40 | ((pos) & 0x3f))
#define LCD_SET_CURSOR(pos)		(0x80 | ((pos) & 0x7f))


void 	vHd44780Cmd(uint8_t byte);
uint8_t ucHd44780DataRead();
void 	vHd44780DataWrite(uint8_t byte);
void 	vHd44780Init();
void 	vHd44780PutString(char *str);

#define vHd44780Clear()			vHd44780Cmd(LCD_CLEAR)
#define vHd44780Home()			vHd44780Cmd(LCD_HOME)
#define vHd44780Mode(d,c,b) 	vHd44780Cmd(LCD_MODE(d,c,b))
#define vHd44780Shift(sc,rl) 	vHd44780Cmd(LCD_SHIFT(sc,rl))
#define vHd44780Cgram(adr)		vHd44780Cmd(LCD_GOTO_CGRAM(adr))
#define vHd44780Ram(adr)		vHd44780Cmd(LCD_SET_CURSOR(adr))
#define vHd44780Pos(x,y)		vHd44780Ram((y<<6) + x)
