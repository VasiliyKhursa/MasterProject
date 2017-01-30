#include "hd44780.h"

#define high(p)		HD44780_PORT |= 1<<HD44780_##p
#define low(p)		HD44780_PORT &= ~(1<<HD44780_##p)

#define nop()		asm("nop")

void vHd44780NibbleWrite(uint8_t nibble)
{
	high(E);
	HD44780_PORT &= ~HD44780_DB_MASK;
	HD44780_PORT |= (nibble&0xf)<<HD44780_DB;
	nop(); nop();
	nop(); nop();
	low(E);
}

uint8_t ucHd44780NibbleRead()
{
	uint8_t nib;

	high(E);
	HD44780_PORT &= ~HD44780_DB_MASK;
	nop(); nop();
	nop(); nop();
	nib = (HD44780_PIN>>HD44780_DB)&0xf;
	low(E);
	return nib;
}

uint8_t ucHd44780BusyRead()
{
	uint8_t byte;

	high(RW);
	HD44780_DDR &= ~HD44780_DB_MASK;
	byte = (ucHd44780NibbleRead()<<4)|
		ucHd44780NibbleRead();
	low(RW);
	HD44780_DDR |= HD44780_DB_MASK;
	return byte&0x80;
}

void vHd44780Cmd(uint8_t byte)
{
	while(ucHd44780BusyRead())
		;

	vHd44780NibbleWrite(byte>>4);
	vHd44780NibbleWrite(byte);
}

uint8_t ucHd44780DataReads()
{
	uint8_t byte;

	while(ucHd44780BusyRead())
		;
	
	HD44780_PORT |= (1<<HD44780_RS)|(1<<HD44780_RW);
	HD44780_DDR &= ~HD44780_DB_MASK;
	byte = (ucHd44780NibbleRead()<<4)|
		ucHd44780NibbleRead();
	HD44780_PORT &= ~((1<<HD44780_RS)|(1<<HD44780_RW));
	HD44780_DDR |= HD44780_DB_MASK;
	return byte;
}

void vHd44780DataWrite(uint8_t byte)
{
	while(ucHd44780BusyRead())
		;

	high(RS);
	vHd44780NibbleWrite(byte>>4);
	vHd44780NibbleWrite(byte);
	low(RS);
}

void vHd44780Init()
{
	HD44780_DDR |= HD44780_DB_MASK|(1<<HD44780_RS)|
		(1<<HD44780_RW)|(1<<HD44780_E);
	
	vHd44780NibbleWrite(3);
	_delay_ms(5);
	vHd44780NibbleWrite(3);
	_delay_us(160);
	vHd44780NibbleWrite(3);
	_delay_us(160);
	vHd44780NibbleWrite(2);
}

void vHd44780PutString(char *str)
{
	while(*str)
		vHd44780DataWrite(*(str++));
}
