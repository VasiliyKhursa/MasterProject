#include <avr/io.h>

#include "uart.h"


//InitUSART
//UBRRL = LO(bauddivider);
//UBRRL = 0x6B;
//UBRRH = HI(bauddivider);
//UBRRH = 0x00;

void InitUART0(unsigned int BAUD, char CharacterSize, char StopBits, char Parity, char Mode)
{
//Mode:  
//	0  -  Asynchronous USART
//	1  -  Synchronous USART
//	2  -  Asynchronous USART double speed
//	3  -  Master SPI (MSPIM)

//Parity
//	0  -  Disabled
//	1  -  Parity bit using even parity
//	2  -  Parity bit using odd parity

//Character Size
//	5  -  5-bit
//	6  -  6-bit
//	7  -  7-bit
//	8  -  8-bit
//	9  -  9-bit

//Stop Bits
//	1  -  1-bit
//	2  -  2-bit

UCSR0A = 0;

//UCSR0A:USART0 Control and Status Register A Ц UCSRA 
 
// Bit 7 Ц RXC : USART Receive Complete 
// Bit 6 Ц TXC : USART Transmit Complete 
// Bit 5 Ц UDRE: USART Data Register Empty 
// Bit 4 Ц FE  : Frame Error 
// Bit 3 Ц DOR : Data OverRun 
// Bit 2 Ц PE  : Parity Error 
// Bit 1 Ц U2X : Double the USART Transmission Speed 
// Bit 0 Ц MPCM: Multi-processor Communication Mode 



UCSR0B = 0<<RXCIE0|0<<TXCIE0|0<<UDRIE0|0<<RXEN0|1<<TXEN0|0<<RXB80|0<<TXB80;

//UCSR0B:USART0 Control and Status Register B Ц UCSRB 
 
// Bit 7 Ц RXCIE : RX Complete Interrupt Enable 
// Bit 6 Ц TXCIE : TX Complete Interrupt Enable 
// Bit 5 Ц UDRIE : USART Data Register Empty Interrupt Enable 
// Bit 4 Ц RXEN  : Receiver Enable 
// Bit 3 Ц TXEN  : Transmitter Enable 
// Bit 2 Ц UCSZ2 : Character Size 
// Bit 1 Ц RXB8  : Receive Data Bit 8 
// Bit 0 Ц TXB8  : Transmit Data Bit 8 




//UCSR0C:USART0 Control and Status Register C Ц UCSRC 

// Bit 7   Ц URSEL: Register Select
// Bit 6   Ц UMSEL: USART Mode Select
// Bit 5:4 Ц UPM1:0: Parity Mode
// Bit 3   Ц USBS: Stop Bit Select
// Bit 2:1 Ц UCSZ1:0: Character Size
// Bit 0   Ц UCPOL: Clock Polarity

	
long int UBBR0=0;

switch (Mode) 
	{ 
		case 0: UBBR0 = F_CPU/16/BAUD-1; 						UCSR0C &= ~(0<<UMSEL01);	UCSR0C &= ~(0<<UMSEL00);	break;		//	асинхронный
	  	case 1:	UBBR0 = F_CPU/8/BAUD-1;  						UCSR0C &= ~(0<<UMSEL01);	UCSR0C |= 1<<UMSEL00;		break;		//	синхронный
	  	case 2: UBBR0 = F_CPU/8/BAUD-1; 	UCSR0A |= 1<<U2X0;	UCSR0C &= ~(0<<UMSEL01);	UCSR0C &= ~(0<<UMSEL00);	break;		//	асинхронный с двойной скоростью
	  	case 3: UBBR0 = F_CPU/2/BAUD-1;  						UCSR0C |= 1<<UMSEL01;	 	UCSR0C |= 1<<UMSEL00;		break;		//	SPI
	}
		
 
		UBRR0L = UBBR0&0x00FF;
		UBRR0H = (UBBR0>>8)&0x00FF;


	
switch (Parity) 
	{ 
		case 0:   UCSR0C &= ~((1<<UPM01)|(1<<UPM00));  	break;  // паритет выкл
		case 1:   break;  										// reserved
	  	case 2:   UCSR0C |= ((1<<UPM01));				UCSR0C &= ~(1<<UPM00);  	break;  // паритет четность
	  	case 3:   UCSR0C |= ((1<<UPM01)|(1<<UPM00));								break;  // паритет нечетность
	}

switch (StopBits) 
	{ 
		case 1:   UCSR0C &= ~(1<<USBS0); 		break;  	// один стоп бит 
	  	case 2:   UCSR0C |= (1<<USBS0);  		break;  	// два  стоп бита 
	}

switch (CharacterSize) 
	{ 
		case 5:   UCSR0B &= ~(1<<UCSZ02);  											UCSR0C &= ~((1<<UCSZ01)|(1<<UCSZ00));  	break;  // 5 бит данных
	  	case 6:   UCSR0B &= ~(1<<UCSZ02); 	UCSR0C |= ((1<<UCSZ00)); 				UCSR0C &= ~(0<<UCSZ01);  				break;  // 6 бит данных
		case 7:   UCSR0B &= ~(1<<UCSZ02); 	UCSR0C |= ((1<<UCSZ01));				UCSR0C &= ~(0<<UCSZ00);   				break;  // 7 бит данных
	  	case 8:   UCSR0B &= ~(1<<UCSZ02); 	UCSR0C |= ((1<<UCSZ01)|(1<<UCSZ00));  											break;  // 8 бит данных
		case 9:   UCSR0B |=   1<<UCSZ02;  	UCSR0C |= ((1<<UCSZ01)|(1<<UCSZ00)); 											break;  // 9 бит данных
	}

	UCSR0C &= ~(1<<UCPOL0);		
//	UCSR0C |= 	1<<UCPOL0;
}





void InitUART1(unsigned int BAUD, char CharacterSize, char StopBits, char Parity, char Mode)
{
//Mode:  
//	0  -  Asynchronous USART
//	1  -  Synchronous USART
//	2  -  Asynchronous USART double speed
//	3  -  Master SPI (MSPIM)

//Parity
//	0  -  Disabled
//	1  -  Parity bit using even parity
//	2  -  Parity bit using odd parity

//Character Size
//	5  -  5-bit
//	6  -  6-bit
//	7  -  7-bit
//	8  -  8-bit
//	9  -  9-bit

//Stop Bits
//	1  -  1-bit
//	2  -  2-bit

UCSR1A = 0;

//UCSR1A:USART1 Control and Status Register A Ц UCSRA 
 
// Bit 7 Ц RXC : USART Receive Complete 
// Bit 6 Ц TXC : USART Transmit Complete 
// Bit 5 Ц UDRE: USART Data Register Empty 
// Bit 4 Ц FE  : Frame Error 
// Bit 3 Ц DOR : Data OverRun 
// Bit 2 Ц PE  : Parity Error 
// Bit 1 Ц U2X : Double the USART Transmission Speed 
// Bit 0 Ц MPCM: Multi-processor Communication Mode 



UCSR1B = 0<<RXCIE1|0<<TXCIE1|0<<UDRIE1|0<<RXEN1|1<<TXEN1|0<<RXB81|0<<TXB81;

//UCSR1B:USART1 Control and Status Register B Ц UCSRB 
 
// Bit 7 Ц RXCIE : RX Complete Interrupt Enable 
// Bit 6 Ц TXCIE : TX Complete Interrupt Enable 
// Bit 5 Ц UDRIE : USART Data Register Empty Interrupt Enable 
// Bit 4 Ц RXEN  : Receiver Enable 
// Bit 3 Ц TXEN  : Transmitter Enable 
// Bit 2 Ц UCSZ2 : Character Size 
// Bit 1 Ц RXB8  : Receive Data Bit 8 
// Bit 0 Ц TXB8  : Transmit Data Bit 8 




//UCSR1C:USART1 Control and Status Register C Ц UCSRC 

// Bit 7   Ц URSEL: Register Select
// Bit 6   Ц UMSEL: USART Mode Select
// Bit 5:4 Ц UPM1:0: Parity Mode
// Bit 3   Ц USBS: Stop Bit Select
// Bit 2:1 Ц UCSZ1:0: Character Size
// Bit 0   Ц UCPOL: Clock Polarity

	
long int UBBR1=0;

switch (Mode) 
	{ 
		case 0: UBBR1 = F_CPU/16/BAUD-1; 						UCSR1C &= ~(0<<UMSEL11);	UCSR1C &= ~(0<<UMSEL10);	break;		//	асинхронный
	  	case 1:	UBBR1 = F_CPU/8/BAUD-1;  						UCSR1C &= ~(0<<UMSEL11);	UCSR1C |= 1<<UMSEL10;		break;		//	синхронный
	  	case 2: UBBR1 = F_CPU/8/BAUD-1; 	UCSR1A |= 1<<U2X1;	UCSR1C &= ~(0<<UMSEL11);	UCSR1C &= ~(0<<UMSEL10);	break;		//	асинхронный с двойной скоростью
	  	case 3: UBBR1 = F_CPU/2/BAUD-1;  						UCSR1C |= 1<<UMSEL11;	 	UCSR1C |= 1<<UMSEL10;		break;		//	SPI
	}
		
 
		UBRR1L = UBBR1&0x00FF;
		UBRR1H = (UBBR1>>8)&0x00FF;


	
switch (Parity) 
	{ 
		case 0:   UCSR1C &= ~((1<<UPM11)|(1<<UPM10));  	break;  // паритет выкл
		case 1:   break;  										// reserved
	  	case 2:   UCSR1C |= ((1<<UPM11));				UCSR1C &= ~(1<<UPM10);  	break;  // паритет четность
	  	case 3:   UCSR1C |= ((1<<UPM11)|(1<<UPM10));								break;  // паритет нечетность
	}

switch (StopBits) 
	{ 
		case 1:   UCSR1C &= ~(1<<USBS1); 		break;  	// один стоп бит 
	  	case 2:   UCSR1C |= (1<<USBS1);  		break;  	// два  стоп бита 
	}

switch (CharacterSize) 
	{ 
		case 5:   UCSR1B &= ~(1<<UCSZ12);  											UCSR1C &= ~((1<<UCSZ11)|(1<<UCSZ10));  	break;  // 5 бит данных
	  	case 6:   UCSR1B &= ~(1<<UCSZ12); 	UCSR1C |= ((1<<UCSZ10)); 				UCSR1C &= ~(0<<UCSZ11);  				break;  // 6 бит данных
		case 7:   UCSR1B &= ~(1<<UCSZ12); 	UCSR1C |= ((1<<UCSZ11));				UCSR1C &= ~(0<<UCSZ10);   				break;  // 7 бит данных
	  	case 8:   UCSR1B &= ~(1<<UCSZ12); 	UCSR1C |= ((1<<UCSZ11)|(1<<UCSZ10));  											break;  // 8 бит данных
		case 9:   UCSR1B |=   1<<UCSZ12;  	UCSR1C |= ((1<<UCSZ11)|(1<<UCSZ10)); 											break;  // 9 бит данных
	}

	UCSR1C &= ~(1<<UCPOL1);		
//	UCSR1C |= 	1<<UCPOL1;
}



void InitUART2(unsigned int BAUD, char CharacterSize, char StopBits, char Parity, char Mode)
{
//Mode:  
//	0  -  Asynchronous USART
//	1  -  Synchronous USART
//	2  -  Asynchronous USART double speed
//	3  -  Master SPI (MSPIM)

//Parity
//	0  -  Disabled
//	1  -  Parity bit using even parity
//	2  -  Parity bit using odd parity

//Character Size
//	5  -  5-bit
//	6  -  6-bit
//	7  -  7-bit
//	8  -  8-bit
//	9  -  9-bit

//Stop Bits
//	1  -  1-bit
//	2  -  2-bit

UCSR2A = 0;

//UCSR2A:USART2 Control and Status Register A Ц UCSRA 
 
// Bit 7 Ц RXC : USART Receive Complete 
// Bit 6 Ц TXC : USART Transmit Complete 
// Bit 5 Ц UDRE: USART Data Register Empty 
// Bit 4 Ц FE  : Frame Error 
// Bit 3 Ц DOR : Data OverRun 
// Bit 2 Ц PE  : Parity Error 
// Bit 1 Ц U2X : Double the USART Transmission Speed 
// Bit 0 Ц MPCM: Multi-processor Communication Mode 



UCSR2B = 0<<RXCIE2|0<<TXCIE2|0<<UDRIE2|0<<RXEN2|1<<TXEN2|0<<RXB82|0<<TXB82;

//UCSR2B:USART2 Control and Status Register B Ц UCSRB 
 
// Bit 7 Ц RXCIE : RX Complete Interrupt Enable 
// Bit 6 Ц TXCIE : TX Complete Interrupt Enable 
// Bit 5 Ц UDRIE : USART Data Register Empty Interrupt Enable 
// Bit 4 Ц RXEN  : Receiver Enable 
// Bit 3 Ц TXEN  : Transmitter Enable 
// Bit 2 Ц UCSZ2 : Character Size 
// Bit 1 Ц RXB8  : Receive Data Bit 8 
// Bit 0 Ц TXB8  : Transmit Data Bit 8 




//UCSR2C:USART2 Control and Status Register C Ц UCSRC 

// Bit 7   Ц URSEL: Register Select
// Bit 6   Ц UMSEL: USART Mode Select
// Bit 5:4 Ц UPM1:0: Parity Mode
// Bit 3   Ц USBS: Stop Bit Select
// Bit 2:1 Ц UCSZ1:0: Character Size
// Bit 0   Ц UCPOL: Clock Polarity

	
long int UBBR2=0;

switch (Mode) 
	{ 
		case 0: UBBR2 = F_CPU/16/BAUD-1; 						UCSR2C &= ~(0<<UMSEL21);	UCSR2C &= ~(0<<UMSEL20);	break;		//	асинхронный
	  	case 1:	UBBR2 = F_CPU/8/BAUD-1;  						UCSR2C &= ~(0<<UMSEL21);	UCSR2C |= 1<<UMSEL20;		break;		//	синхронный
	  	case 2: UBBR2 = F_CPU/8/BAUD-1; 	UCSR2A |= 1<<U2X2;	UCSR2C &= ~(0<<UMSEL21);	UCSR2C &= ~(0<<UMSEL20);	break;		//	асинхронный с двойной скоростью
	  	case 3: UBBR2 = F_CPU/2/BAUD-1;  						UCSR2C |= 1<<UMSEL21;	 	UCSR2C |= 1<<UMSEL20;		break;		//	SPI
	}
		
 
		UBRR2L = UBBR2&0x00FF;
		UBRR2H = (UBBR2>>8)&0x00FF;


	
switch (Parity) 
	{ 
		case 0:   UCSR2C &= ~((1<<UPM21)|(1<<UPM20));  	break;  // паритет выкл
		case 1:   break;  										// reserved
	  	case 2:   UCSR2C |= ((1<<UPM21));				UCSR2C &= ~(1<<UPM20);  	break;  // паритет четность
	  	case 3:   UCSR2C |= ((1<<UPM21)|(1<<UPM20));								break;  // паритет нечетность
	}

switch (StopBits) 
	{ 
		case 1:   UCSR2C &= ~(1<<USBS2); 		break;  	// один стоп бит 
	  	case 2:   UCSR2C |= (1<<USBS2);  		break;  	// два  стоп бита 
	}

switch (CharacterSize) 
	{ 
		case 5:   UCSR2B &= ~(1<<UCSZ22);  											UCSR2C &= ~((1<<UCSZ21)|(1<<UCSZ20));  	break;  // 5 бит данных
	  	case 6:   UCSR2B &= ~(1<<UCSZ22); 	UCSR2C |= ((1<<UCSZ20)); 				UCSR2C &= ~(0<<UCSZ21);  				break;  // 6 бит данных
		case 7:   UCSR2B &= ~(1<<UCSZ22); 	UCSR2C |= ((1<<UCSZ21));				UCSR2C &= ~(0<<UCSZ20);   				break;  // 7 бит данных
	  	case 8:   UCSR2B &= ~(1<<UCSZ22); 	UCSR2C |= ((1<<UCSZ21)|(1<<UCSZ20));  											break;  // 8 бит данных
		case 9:   UCSR2B |=   1<<UCSZ22;  	UCSR2C |= ((1<<UCSZ21)|(1<<UCSZ20)); 											break;  // 9 бит данных
	}

	UCSR2C &= ~(1<<UCPOL2);		
//	UCSR2C |= 	1<<UCPOL2;
}


void InitUART3(unsigned int BAUD, char CharacterSize, char StopBits, char Parity, char Mode)
{
//Mode:  
//	0  -  Asynchronous USART
//	1  -  Synchronous USART
//	2  -  Asynchronous USART double speed
//	3  -  Master SPI (MSPIM)

//Parity
//	0  -  Disabled
//	1  -  Parity bit using even parity
//	2  -  Parity bit using odd parity

//Character Size
//	5  -  5-bit
//	6  -  6-bit
//	7  -  7-bit
//	8  -  8-bit
//	9  -  9-bit

//Stop Bits
//	1  -  1-bit
//	2  -  2-bit

UCSR3A = 0;

//UCSR3A:USART3 Control and Status Register A Ц UCSRA 
 
// Bit 7 Ц RXC : USART Receive Complete 
// Bit 6 Ц TXC : USART Transmit Complete 
// Bit 5 Ц UDRE: USART Data Register Empty 
// Bit 4 Ц FE  : Frame Error 
// Bit 3 Ц DOR : Data OverRun 
// Bit 2 Ц PE  : Parity Error 
// Bit 1 Ц U2X : Double the USART Transmission Speed 
// Bit 0 Ц MPCM: Multi-processor Communication Mode 



UCSR3B = 0<<RXCIE3|0<<TXCIE3|0<<UDRIE3|0<<RXEN3|1<<TXEN3|0<<RXB83|0<<TXB83;

//UCSR3B:USART3 Control and Status Register B Ц UCSRB 
 
// Bit 7 Ц RXCIE : RX Complete Interrupt Enable 
// Bit 6 Ц TXCIE : TX Complete Interrupt Enable 
// Bit 5 Ц UDRIE : USART Data Register Empty Interrupt Enable 
// Bit 4 Ц RXEN  : Receiver Enable 
// Bit 3 Ц TXEN  : Transmitter Enable 
// Bit 2 Ц UCSZ2 : Character Size 
// Bit 1 Ц RXB8  : Receive Data Bit 8 
// Bit 0 Ц TXB8  : Transmit Data Bit 8 




//UCSR3C:USART3 Control and Status Register C Ц UCSRC 

// Bit 7   Ц URSEL: Register Select
// Bit 6   Ц UMSEL: USART Mode Select
// Bit 5:4 Ц UPM1:0: Parity Mode
// Bit 3   Ц USBS: Stop Bit Select
// Bit 2:1 Ц UCSZ1:0: Character Size
// Bit 0   Ц UCPOL: Clock Polarity

	
long int UBBR3=0;

switch (Mode) 
	{ 
		case 0: UBBR3 = F_CPU/16/BAUD-1; 						UCSR3C &= ~(0<<UMSEL31);	UCSR3C &= ~(0<<UMSEL30);	break;		//	асинхронный
	  	case 1:	UBBR3 = F_CPU/8/BAUD-1;  						UCSR3C &= ~(0<<UMSEL31);	UCSR3C |= 1<<UMSEL30;		break;		//	синхронный
	  	case 2: UBBR3 = F_CPU/8/BAUD-1; 	UCSR3A |= 1<<U2X3;	UCSR3C &= ~(0<<UMSEL31);	UCSR3C &= ~(0<<UMSEL30);	break;		//	асинхронный с двойной скоростью
	  	case 3: UBBR3 = F_CPU/2/BAUD-1;  						UCSR3C |= 1<<UMSEL31;	 	UCSR3C |= 1<<UMSEL30;		break;		//	SPI
	}
		
 
		UBRR3L = UBBR3&0x00FF;
		UBRR3H = (UBBR3>>8)&0x00FF;


	
switch (Parity) 
	{ 
		case 0:   UCSR3C &= ~((1<<UPM31)|(1<<UPM30));  	break;  // паритет выкл
		case 1:   break;  										// reserved
	  	case 2:   UCSR3C |= ((1<<UPM31));				UCSR3C &= ~(1<<UPM30);  	break;  // паритет четность
	  	case 3:   UCSR3C |= ((1<<UPM31)|(1<<UPM30));								break;  // паритет нечетность
	}

switch (StopBits) 
	{ 
		case 1:   UCSR3C &= ~(1<<USBS3); 		break;  	// один стоп бит 
	  	case 2:   UCSR3C |= (1<<USBS3);  		break;  	// два  стоп бита 
	}

switch (CharacterSize) 
	{ 
		case 5:   UCSR3B &= ~(1<<UCSZ32);  											UCSR3C &= ~((1<<UCSZ31)|(1<<UCSZ30));  	break;  // 5 бит данных
	  	case 6:   UCSR3B &= ~(1<<UCSZ32); 	UCSR3C |= ((1<<UCSZ30)); 				UCSR3C &= ~(0<<UCSZ31);  				break;  // 6 бит данных
		case 7:   UCSR3B &= ~(1<<UCSZ32); 	UCSR3C |= ((1<<UCSZ31));				UCSR3C &= ~(0<<UCSZ30);   				break;  // 7 бит данных
	  	case 8:   UCSR3B &= ~(1<<UCSZ32); 	UCSR3C |= ((1<<UCSZ31)|(1<<UCSZ30));  											break;  // 8 бит данных
		case 9:   UCSR3B |=   1<<UCSZ32;  	UCSR3C |= ((1<<UCSZ31)|(1<<UCSZ30)); 											break;  // 9 бит данных
	}

	UCSR3C &= ~(1<<UCPOL3);		
//	UCSR3C |= 	1<<UCPOL3;
}





void SendToUart0(unsigned char Data)
{
	UDR0 = Data;
	
	while(!( UCSR0A & (1<<UDRE0)));
} 

void SendToUart1(unsigned char Data)
{
	UDR1 = Data;
	
	while(!( UCSR1A & (1<<UDRE1)));
}

void SendToUart2(unsigned char Data)
{
	UDR2 = Data;
	
	while(!( UCSR2A & (1<<UDRE2)));
}  

void SendToUart3(unsigned char Data)
{
	UDR3 = Data;
	
	while(!( UCSR3A & (1<<UDRE3)));
} 

unsigned char ReceiveToUart0(void)
{
	unsigned char data, status;
	
	while(!(UCSR0A & (1<<RXC0))); 	// Wait for incomming data
	
	status = UCSR0A;
	data = UDR0;
	
	return(data);
}

void transmitStringToUart(uint8_t uart_numb, unsigned char* string)
{
  switch(uart_numb) {
  case 0:  while (*string) SendToUart0(*string++);
  case 1:  while (*string) SendToUart1(*string++);
  case 2:  while (*string) SendToUart2(*string++);
  case 3:  while (*string) SendToUart3(*string++);
  default: while (*string) SendToUart0(*string++);
 }
}
