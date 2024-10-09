#include "i2c.h"
#include "ds1307.h"
#include "lcd.h"

#include <avr/io.h>

extern unsigned char hour, minute, second, date, month;
extern unsigned int  year;

// Функция чтения данных из DS1307
uint8_t ucDS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res; // Результат
	I2CStart(); // СТАРТ
	res = I2CWriteByte(0b11010000);	// адрес DS1307 + бит W
	if(!res)	return 0; // ОШИБКА
	// Передача адреса необходимого регистра
	res = I2CWriteByte(address);
	if(!res)	return 0; // ОШИБКА	
	I2CStart(); // Повторный СТАРТ	
	res = I2CWriteByte(0b11010001);	// адрес DS1307 + бит R
	if(!res)	return 0; // ОШИБКА	
	// Чтение данных с неподтверждением
	res = I2CReadByte(data,0);
	if(!res)	return 0; // ОШИБКА
	I2CStop(); // СТОП
	return 1;
}

// Функция записи данных в DS1307
uint8_t ucDS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res; // Результат
	I2CStart();	// СТАРТ
	res = I2CWriteByte(0b11010000);	// адрес DS1307 + бит W
	if(!res)	return 0; // ОШИБКА	
	// Передача адреса необходимого регистра
	res = I2CWriteByte(address);
	if(!res)	return 0; // ОШИБКА
	res = I2CWriteByte(data); // Запись данных
	if(!res)	return 0; // ОШИБКА
	I2CStop(); // СТОП
	return 1;
}




//-----------------------------------------Функции часов------------------------------------------//

void vDS1307StartTime(void)
{
	// Запускаем ход часов
	uint8_t temp;
	ucDS1307Read(0x00,&temp);

	temp &= ~(1 << 7); 			// обнуляєм Bit7 регистра 0x00
	ucDS1307Write(0x00,temp);

}

void vDS1307StoptTime(void)
{
	// Останавливаем ход часов
	uint8_t temp;
	ucDS1307Read(0x00,&temp);

	temp |= (1 << 7); 			// устанавливаем Bit7 регистра 0x00 
	ucDS1307Write(0x00,temp);

}

void vDS1307SetTime(unsigned char hour, unsigned char min, unsigned char seconds, unsigned char format)
{
	// hour = 0..23
	// min = 0..59
	// seconds = 0..59
	// format = 1 - 12 часов
	// format = 0 - 24 часа
	
	unsigned char temp, seconds_tens, seconds_ed, minutes_tens, minutes_ed, hours_tens, hours_ed;
//	DS1307Read(0x00,&temp);
	

	seconds_tens = seconds/10;		//	- получаем десятки секунд
	seconds_ed = seconds%10;		//	- получает единици секунд

	temp = ((seconds_tens << 4) + seconds_ed);
	ucDS1307Write(0x00,temp);

	minutes_tens = min/10;			//	- получаем десятки минут
	minutes_ed = min%10;			//	- получает единици минут

	temp = ((minutes_tens << 4) + minutes_ed);
	ucDS1307Write(0x01,temp);

	hours_tens = hour/10;			//	- получаем десятки часов
	hours_ed = hour%10;				//	- получает единици часов

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
	
	temp = (day);						//	- записываем дату 
	ucDS1307Write(0x03,temp);

	date_tens = date/10;				//	- получаем десятки дня
	date_ed = date%10;					//	- получает единици дня

	temp = ((date_tens << 4) + date_ed);
	ucDS1307Write(0x04,temp);

	month_tens = month/10;				//	- получаем десятки месяца
	month_ed = month%10;				//	- получает единици месяца

	temp = ((month_tens << 4) + month_ed);		
	ucDS1307Write(0x05,temp);

	year_tens = year/10;				//	- получаем десятки года
	year_ed = year%10;					//	- получает единици года

	temp = ((year_tens << 4) + year_ed);		
	ucDS1307Write(0x06,temp);
}


void vDS1307TimeReadWriteLCD(void)
{
	uint8_t temp;
	// Читаем данные и преобразуем из BCD в двоичную систему

	ucDS1307Read(0x00,&temp); 			// Чтение регистра секунд
	second = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x01,&temp); 			// Чтение регистра минут
	minute = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x02,&temp); 			// Чтение регистра часов
	hour = (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	vLcdWriteIntXY(6,0,hour,2);
	vLcdWriteIntXY(9,0,minute,2);
	vLcdWriteIntXY(12,0,second,2);

	_delay_ms(100);

}

void vDS1307DateReadWriteLCD(void)
{
	uint8_t temp;
	// Читаем данные и преобразуем из BCD в двоичную систему
	ucDS1307Read(0x04,&temp); 			// Чтение регистра дней
	date = (((temp & 0x30) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x05,&temp); 			// Чтение регистра месяцев
	month = (((temp & 0x10) >> 4)*10)+(temp & 0x0F);
	
	ucDS1307Read(0x06,&temp); 			// Чтение регистра годов
	year = 2000 + (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);

	vLcdWriteIntXY(6,1,date,2);
	vLcdWriteIntXY(9,1,month,2);
	vLcdWriteIntXY(12,1,year,4);

	_delay_ms(100);

}


