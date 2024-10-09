#ifndef SPILS020_H
#define SPILS020_H

#define 	DD_MOSI 	PB2
#define 	DD_MISO 	PB3
#define 	DD_SCK 		PB1
#define 	DD_SS 		PB4
#define 	DDR_SPI 	DDRB
#define 	PORT_SPI	PORTB


extern void InitSPI(char Mode, char Setings, char DivFrequency, char Direction);
extern void SendToSPI8bit(unsigned char Data8);
extern void SendToSPI16bit(int Data16);
extern void SlaveSPISelectON(void);
extern void SlaveSPISelectOFF(void);
extern void SPION(void);
extern void SPIOFF(void);
extern void SPIPINON(void);
extern void SPIPINOFF(void);
extern void SPIInterruptON(void);
extern void SPIInterruptOFF(void);

extern unsigned char ReceiveFromSPI8bit(void);


#endif
