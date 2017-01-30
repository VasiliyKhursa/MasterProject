#ifndef DS1307_H
#define DS1307_H

#include <avr/io.h>

uint8_t ucDS1307Read(uint8_t address,uint8_t *data);
uint8_t ucDS1307Write(uint8_t address,uint8_t data);
void 	vDS1307StartTime(void);
void 	vDS1307StoptTime(void);
void 	vDS1307SetTime(unsigned char hour, unsigned char min, unsigned char seconds, unsigned char format);
void 	vDS1307SetData(unsigned char day, unsigned char date, unsigned char month, unsigned char year);
void 	vDS1307TimeReadWriteLCD(void);
void 	vDS1307DateReadWriteLCD(void);

#endif
