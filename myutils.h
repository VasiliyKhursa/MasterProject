#ifndef MYUTILS_H
#define MYUTILS_H

	#define _CONCAT(a,b) a##b
	#define PORT(x) _CONCAT(PORT,x)
	#define PIN(x) _CONCAT(PIN,x)
	#define DDR(x) _CONCAT(DDR,x)

	#define TASCII(numb) (numb+48)

	#define true 1
	#define false 0

	#define vuchar volatile unsigned char 
	#define vuint volatile unsigned int

	#define sbi(reg,bit) reg |= (1<<bit)
	#define cbi(reg,bit) reg &= ~(1<<bit)
	#define ibi(reg,bit) reg ^= (1<<bit)
	#define CheckBit(reg,bit) (reg&(1<<bit))

	void RunTasks(void);


#endif
