#include "spils020.h"
#include <avr/io.h>

void InitSPI(char Mode, char Setings, char DivFrequency, char Direction)
{

 	SPIPINON();

//Mode:  
//	0  -  Master
//	1  -  Slave 

//Setings
//			CPOL	CPHA	CPOL:	Leading Edge 	Trailing Edge		CPHA:	Leading Edge 	Trailing Edge
//	0  -  	 0		  0					Rising			Falling						Sample			Setup
//	1  -  	 0		  1					Rising			Falling						Setup			Sample			
//	2  -     1        0					Falling			Rising						Sample			Setup		
//	3  -  	 1        1					Falling			Rising						Setup			Sample

//DivFrequency
//	0  -  Fosc/4
//	1  -  Fosc/16
//	2  -  Fosc/64
//	3  -  Fosc/128
//	4  -  Fosc/2
//	5  -  Fosc/8
//	6  -  Fosc/32
//	7  -  Fosc/64

//Direction
//	0  -  MSB of the data word is transmitted first
//	1  -  LSB of the data word is transmitted first

	SPCR = 0x00;
	SPSR = 0x00;

//SPCR:SPI Control Register – SPCR 
// Bit 7 – SPIE: SPI Interrupt Enable 
// Bit 6 – SPE: SPI Enable 
// Bit 5 – DORD: Data Order 
// Bit 4 – MSTR: Master/Slave Select 
// Bit 3 – CPOL: Clock Polarity 
// Bit 2 – CPHA: Clock Phase 
// Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0 

	SPSR = 1<<SPIF|0<<WCOL|0<<SPI2X;

//SPSR:SPI Status Register – SPSR 
// Bit 7 – SPIF: SPI Interrupt Flag 
// Bit 6 – WCOL: Write COLlision flag 
// Bit 5..1 – Res: Reserved Bits 
// Bit 0 – SPI2X: Double SPI Speed Bit 


switch (Mode) 
	{ 
		case 0:  SPCR |= 1<<MSTR;			DDR_SPI |= 1<<DD_SS; 	SlaveSPISelectOFF();break;		//Master
	  	case 1:	 SPCR &= ~(1<<MSTR);		DDR_SPI &= ~(1<<DD_SS);						break;		//Slave
	}

switch (Setings) 
	{ 
		case 0:	 SPCR &= ~((1<<CPOL)|(1<<CPHA));  					break;  
	  	case 1:  SPCR &= ~(1<<CPOL); 			SPCR |= 1<<CPHA;	break;  
	  	case 2:  SPCR &= ~(1<<CPHA); 			SPCR |= 1<<CPOL;	break;  
	  	case 3:  SPCR |=  (1<<CPOL)|(1<<CPHA); 						break;  
	}

switch (DivFrequency) 
	{ 
		case 0:	 SPSR &= ~(1<<SPI2X);  			SPCR &= ~((1<<SPR1)|(1<<SPR0));							break;  
	  	case 1:	 SPSR &= ~(1<<SPI2X);  			SPCR &= ~(1<<SPR1); 			SPCR |= 1<<SPR0;		break; 
	  	case 2:  SPSR &= ~(1<<SPI2X);  			SPCR &= ~(1<<SPR0); 			SPCR |= 1<<SPR1;		break; 
	  	case 3:  SPSR &= ~(1<<SPI2X);  			SPCR |= (1<<SPR1)|(1<<SPR0);							break; 
		case 4:	 SPSR |= 1<<SPI2X;  			SPCR &= ~((1<<SPR1)|(1<<SPR0));							break;  
	  	case 5:	 SPSR |= 1<<SPI2X;  			SPCR &= ~(1<<SPR1); 			SPCR |= 1<<SPR0;		break; 
	  	case 6:  SPSR |= 1<<SPI2X;  			SPCR &= ~(1<<SPR0); 			SPCR |= 1<<SPR1;		break; 
	  	case 7:  SPSR |= 1<<SPI2X;  			SPCR |= (1<<SPR1)|(1<<SPR0);							break;
	}

switch (Direction) 
	{ 
		case 0:  SPCR &= ~(1<<DORD);	break;		// MSB first
	  	case 1:	 SPCR |= 1<<DORD;		break;		// LSB first
	}

	SPION();

}

void SPIPINON(void)
{
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK);				// pin out
	DDR_SPI &= ~(1<<DD_MISO);							// pin in
}

void SPIPINOFF(void)
{
	DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS)|(1<<DD_MISO));		// pin in
}

void SPIInterruptON(void)
{
	SPCR |= 1<<SPIE;	
}

void SPIInterruptOFF(void)
{
	SPCR &= ~(1<<SPIE);	
}

void SPION(void)
{
	SPCR |= 1<<SPE;				// SPI on
}

void SPIOFF(void)
{
	SPCR &= ~(1<<SPE);			// SPI off
}


void SlaveSPISelectON(void)
{
	PORT_SPI &= ~(1<<DD_SS);
}

void SlaveSPISelectOFF(void)
{
	PORT_SPI |= 1<<DD_SS;
}


void SendToSPI8bit(unsigned char Data8)
{
	SlaveSPISelectON();
	SPDR = Data8;
	while(!(SPSR & (1<<SPIF))){}
	SlaveSPISelectOFF();
}


void SendToSPI16bit(int Data16)
{
	SendToSPI8bit((8>>Data16)&0x00FF);		// send elder byte 
	SendToSPI8bit(Data16&0x00FF);			// send younger byte
}


unsigned char ReceiveFromSPI8bit(void)
{
	while(!(SPSR & (1<<SPIF))){}
	return SPDR;
}

