#include "lcd2.h"
#include <avr/io.h>
#include <util/delay.h>


// Функция передачи команды
void lcd_com(unsigned char p)
{
	PORTH &= ~(1 << RS); // RS = 0 (запись команд)
	PORTH |= (1 << EN); // EN = 1 (начало записи команды в LCD)
	PORTB &= 0x0F; PORTB |= (p & 0xF0); // старший нибл
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (конец записи команды в LCD)
	_delay_us(100);
	PORTH |= (1 << EN); // EN = 1 (начало записи команды в LCD)
	PORTB &= 0x0F; PORTB |= (p << 4); // младший нибл
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (конец записи команды в LCD)
	_delay_us(100);
}

// Функция передачи данных
void lcd_data(unsigned char p)
{
	PORTH |= (1 << RS)|(1 << EN); // RS = 1 (запись данных), EN - 1 (начало записи команды в LCD)
	PORTB &= 0x0F; PORTB |= (p & 0xF0); // старший нибл
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (конец записи команды в LCD)
	_delay_us(100);
	PORTH |= (1 << EN); // EN = 1 (начало записи команды в LCD)
	PORTB &= 0x0F; PORTB |= (p << 4); // младший нибл
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (конец записи команды в LCD)
	_delay_us(100);
}

// Функция вывода строки на LCD
void lcd_string(unsigned char command, char *string)
{
	lcd_com(0x0C);
	lcd_com(command);
	while(*string != '\0')
	{  lcd_data(*string);
	string++;
	}
}

// Функция вывода переменной
void lcd_num_to_str(unsigned int value, unsigned char nDigit)
{
	switch(nDigit)
	{
	case 4: lcd_data((value/1000)+'0');
	case 3: lcd_data(((value/100)%10)+'0');
	case 2: lcd_data(((value/10)%10)+'0');
	case 1: lcd_data((value%10)+'0');
	}
}

// Функция инициализации LCD
void lcd_init(void)
{
	PORTB = 0x00;
	DDRB = 0xFF;
	PORTH = 0x00;
	DDRH = 0xFF;

	_delay_ms(50); // Ожидание готовности ЖК-модуля

	// Конфигурирование четырехразрядного режима
	PORTB |= (1 << PD5);
	PORTB &= ~(1 << PD4);

	// Активизация четырехразрядного режима
	PORTH |= (1 << EN);
	PORTH &= ~(1 << EN);
	_delay_ms(5); 

	lcd_com(0x28); // шина 4 бит, LCD - 2 строки
	lcd_com(0x08); // полное выключение дисплея
	lcd_com(0x01); // очистка дисплея
	_delay_us(100);
	lcd_com(0x06); // сдвиг курсора вправо
	lcd_com(0x0C); // включение дисплея, курсор не видим
}
