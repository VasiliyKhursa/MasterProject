#include "i2c.h"
#include "ds1307.h"
#include "lcd.h"

#include <avr/io.h>

extern unsigned char hour, minute, second, date, month;
extern unsigned int  year;

// ������� ������ ������ �� DS1307
uint8_t ucDS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res; // ���������
	I2CStart(); // �����
	res = I2CWriteByte(0b11010000);	// ����� DS1307 + ��� W
	if(!res)	return 0; // ������
	// �������� ������ ������������ ��������
	res = I2CWriteByte(address);
	if(!res)	return 0; // ������	
	I2CStart(); // ��������� �����	
	res = I2CWriteByte(0b11010001);	// ����� DS1307 + ��� R
	if(!res)	return 0; // ������	
	// ������ ������ � ����������������
	res = I2CReadByte(data,0);
	if(!res)	return 0; // ������
	I2CStop(); // ����
	return 1;
}

// ������� ������ ������ � DS1307
uint8_t ucDS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res; // ���������
	I2CStart();	// �����
	res = I2CWriteByte(0b11010000);	// ����� DS1307 + ��� W
	if(!res)	return 0; // ������	
	// �������� ������ ������������ ��������
	res = I2CWriteByte(address);
	if(!res)	return 0; // ������
	res = I2CWriteByte(data); // ������ ������
	if(!res)	return 0; // ������
	I2CStop(); // ����
	return 1;
}




//-----------------------------------------������� �����------------------------------------------//

void vDS1307StartTime(void)
{
	// ��������� ��� �����
	uint8_t temp;
	ucDS1307Read(0x00,&temp);

	temp &= ~(1 << 7); 			// �������� Bit7 �������� 0x00
	ucDS1307Write(0x00,temp);

}

void vDS1307StoptTime(void)
{
	// ������������� ��� �����
	uint8_t temp;
	ucDS1307Read(0x00,&temp);

	temp |= (1 << 7); 			// ������������� Bit7 �������� 0x00 
	ucDS1307Write(0x00,temp);

}

void vDS1307SetTime(unsigned char hour, unsigned char min, unsigned char seconds, unsigned char format)
{
	// hour = 0..23
	// min = 0..59
	// seconds = 0..59
	// format = 1 - 12 �����
	// format = 0 - 24 ����
	
	unsigned char temp, seconds_tens, seconds_ed, minutes_tens, minutes_ed, hours_tens, hours_ed;
//	DS1307Read(0x00,&temp);
	

	seconds_tens = seconds/10;		//	- �������� ������� ������
	seconds_ed = seconds%10;		//	- �������� ������� ������

	temp = ((seconds_tens << 4) + seconds_ed);
	ucDS1307Write(0x00,temp);

	minutes_tens = min/10;			//	- �������� ������� �����
	minutes_ed = min%10;			//	- �������� ������� �����

	temp = ((minutes_tens << 4) + minutes_ed);
	ucDS1307Write(0x01,temp);

	hours_tens = hour/10;			//	- �������� ������� �����
	hours_ed = hour%10;				//	- �������� ������� �����

	temp = ((format << 6) + (hours_tens << 4) + hours_ed);		
	ucDS1307Write(0x02,temp);


}

void vDS1307SetData(unsigned char day, unsigned char date, unsigned char month, unsigned char year)
{
	// day 		= 01..07
	// date 	= 01..31
	// month 	= 01..12
	// year 	= 00..99

	
	unsigned char temp, date_tens, date_ed, month_tens, month_ed, year_tens, year_ed;
	
	temp = (day);						//	- ���������� ���� 
	ucDS1307Write(0x03,temp);

	date_tens = date/10;				//	- �������� ������� ���
	date_ed = date%10;					//	- �������� ������� ���

	temp = ((date_tens << 4) + date_ed);
	ucDS1307Write(0x04,temp);

	month_tens = month/10;				//	- �������� ������� ������
	month_ed = month%10;				//	- �������� ������� ������

	temp = ((month_tens << 4) + month_ed);		
	ucDS1307Write(0x05,temp);

	year_tens = year/10;				//	- �������� ������� ����
	year_ed = year%10;					//	- �������� ������� ����

	temp = ((year_tens << 4) + year_ed);		
	ucDS1307Write(0x06,temp);
}


void vDS1307TimeReadWriteLCD(void)
{
	uint8_t temp;
	// ������ ������ � ����������� �� BCD � �������� �������

	ucDS1307Read(0x00,&temp); 			// ������ �������� ������
	second = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x01,&temp); 			// ������ �������� �����
	minute = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x02,&temp); 			// ������ �������� �����
	hour = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	vLcdWriteIntXY(6,0,hour,2);
	vLcdWriteIntXY(9,0,minute,2);
	vLcdWriteIntXY(12,0,second,2);

	_delay_ms(100);

}

void vDS1307DateReadWriteLCD(void)
{
	uint8_t temp;
	// ������ ������ � ����������� �� BCD � �������� �������
	ucDS1307Read(0x04,&temp); 			// ������ �������� ����
	date = (((temp & 0x30) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x05,&temp); 			// ������ �������� �������
	month = (((temp & 0x10) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x06,&temp); 			// ������ �������� �����
	year = 2000 + (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	vLcdWriteIntXY(6,1,date,2);
	vLcdWriteIntXY(9,1,month,2);
	vLcdWriteIntXY(12,1,year,4);

	_delay_ms(100);

}


