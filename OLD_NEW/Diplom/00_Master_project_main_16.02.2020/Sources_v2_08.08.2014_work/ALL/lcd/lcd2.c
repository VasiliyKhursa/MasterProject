#include "lcd2.h"
#include <avr/io.h>
#include <util/delay.h>


// ������� �������� �������
void lcd_com(unsigned char p)
{
	PORTH &= ~(1 << RS); // RS = 0 (������ ������)
	PORTH |= (1 << EN); // EN = 1 (������ ������ ������� � LCD)
	PORTB &= 0x0F; PORTB |= (p & 0xF0); // ������� ����
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (����� ������ ������� � LCD)
	_delay_us(100);
	PORTH |= (1 << EN); // EN = 1 (������ ������ ������� � LCD)
	PORTB &= 0x0F; PORTB |= (p << 4); // ������� ����
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (����� ������ ������� � LCD)
	_delay_us(100);
}

// ������� �������� ������
void lcd_data(unsigned char p)
{
	PORTH |= (1 << RS)|(1 << EN); // RS = 1 (������ ������), EN - 1 (������ ������ ������� � LCD)
	PORTB &= 0x0F; PORTB |= (p & 0xF0); // ������� ����
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (����� ������ ������� � LCD)
	_delay_us(100);
	PORTH |= (1 << EN); // EN = 1 (������ ������ ������� � LCD)
	PORTB &= 0x0F; PORTB |= (p << 4); // ������� ����
	_delay_us(100);
	PORTH &= ~(1 << EN); // EN = 0 (����� ������ ������� � LCD)
	_delay_us(100);
}

// ������� ������ ������ �� LCD
void lcd_string(unsigned char command, char *string)
{
	lcd_com(0x0C);
	lcd_com(command);
	while(*string != '\0')
	{  lcd_data(*string);
	string++;
	}
}

// ������� ������ ����������
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

// ������� ������������� LCD
void lcd_init(void)
{
	PORTB = 0x00;
	DDRB = 0xFF;
	PORTH = 0x00;
	DDRH = 0xFF;

	_delay_ms(50); // �������� ���������� ��-������

	// ���������������� ����������������� ������
	PORTB |= (1 << PD5);
	PORTB &= ~(1 << PD4);

	// ����������� ����������������� ������
	PORTH |= (1 << EN);
	PORTH &= ~(1 << EN);
	_delay_ms(5); 

	lcd_com(0x28); // ���� 4 ���, LCD - 2 ������
	lcd_com(0x08); // ������ ���������� �������
	lcd_com(0x01); // ������� �������
	_delay_us(100);
	lcd_com(0x06); // ����� ������� ������
	lcd_com(0x0C); // ��������� �������, ������ �� �����
}
