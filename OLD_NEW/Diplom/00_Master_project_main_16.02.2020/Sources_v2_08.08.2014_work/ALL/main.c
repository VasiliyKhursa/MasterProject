//---------------------------------------Info--------------------------------------//
/*
	Device 			-	Atmega2560
  	Extern ADC		-	MCP320X
	LCD				-	1602
	Ethenet			- 	ENC28J60
	RTC				-	DS1307
	Temp			-	DS18B20
	Freq 			- 	12.0MHz
	RTOS			-	FreeRTOS v 8.0.0

//Распиновка 
 	
	UART0: BAUDRATE 9600	// PC
	UART1: BAUDRATE xxxx	// LS020
	UART2: BAUDRATE 9600	// GSM

	SPI:
		* CS_Extern_ADC	-> PB0
		* SCK 			-> PB1
		* MOSI 			-> PB2
		* MISO 			-> PB3
		* CS_ENC28J60	-> PB4
		* CS_LS020		-> PB5
		* CS_SD_Card	-> PG0

						
	ENC28J60:
		* CS_LS020		-> PB5
		* Reset			-> PB6
		* RS			-> PB7


	LS020:
		* CS_ENC28J60	-> PB4
		* Reset			-> 
		* INT			->
		* WOL			-> 

	RTC:	
		* SCL			-> PD0
		* SDA			-> PD1
		* Battery		-> 
		* SOUT			-> 

	One-Wire:	
		* DQ			-> PG2 
							
	LCD:
		* RS 			-> PA0
		* R/W 			-> PA1
		* E				-> PA2
		* D4			-> PA3
		* D5			-> PA4
		* D6			-> PA5
		* D7			-> PA6


	UART0:
		* RX0			-> PE0
		* TX0			-> PE1
				
	UART2:
		* RX0			-> PH0
		* TX0			-> PH1		

	I2C:
		* SCL			-> PD0
		* SDA			-> PD1


	Leds:

		* Green1 		-> PE4
		* Green2 		-> PE3
		* Red6 			-> PG1


	Extern ADC:		  ADC_MK		PIN_MK

U2	Channel_0		Channel_12		 PK4		// Напряжение 2
A2	Channel_1		Channel_13		 PK5		// Ток 2
U1	Channel_2		Channel_14		 PK6		// Напряжение 1
A1	Channel_3		Channel_15		 PK7		// Ток 1



*/


//----------------STANDART-------------//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>       

//------------------FAT----------------//
#include "integer.h"
#include "diskio.h"
#include "ff.h"

//------------------UART---------------//
#include "UART_routines.h"					// for FF
#include "uart.h"
#include "AT_COMMAND.h"

//------------------SPI----------------//
#include "spi.h"							// for AdcEx
#include "spils020.h"

//------------------ADC----------------//
#include "adc.h"
#include "adc_ex.h"
#include "myutils.h"

//------------------LCD----------------//
#include "lcd.h"							// for time
#include "hd44780.h"						// for LAN

//#include "informmodels.h"
#include "ls020.h"

//------------------I2C----------------//
#include "i2c.h"
#include "ds1307.h"

//----------------One-Wire-------------//
#include "onewire.h"
#include "ds18x20.h"

//------------------Lan----------------//
#include "enc28j60.h"
#include "lan.h"
#include "ntp.h"

//---------------FreeRTOS--------------//
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "freeRTOSBoardDefs.h"

//-----------------------------------Константы для файловой системы------------------------------------------------

char dataFAT []  = "30.299	2.001	29.311	4.999	35.125	1000.999	12:40:13	12.10.1991\n\r";		// строка записи в память

unsigned char hour, minute, second, date, month;									// глобальные переменные времени и даты
unsigned int  year;																	// глобальная переменная для хранения года (1991)

unsigned char temp;

float VoltagePower = 4.7;															// глобальная переменная для напряжения питания АЦП и опорного напряжения

unsigned char ADC_Hlobal;															// глобальная переменная для хранения данных с АЦП

FILE usart_str = FDEV_SETUP_STREAM(SendToUart0, NULL, _FDEV_SETUP_WRITE); 			// для функции printf
#define xmit(a) {transmitString((unsigned char*)a);TX_NEWLINE;}


char buf[100];
static FRESULT f_err_code;
static FATFS FATFS_Obj;
UINT ByteWrite = 0;

char* var="Данные";
char* var1[100];

UINT ByteRead = 0;

//	A varriable to hold the converted value.
//	uint16_t result;

DWORD get_fattime ()
{
	return	  ((DWORD)(/*rtc.year*/2010 - 1980) << 25)
			| ((DWORD)/*rtc.month*/10 << 21)
			| ((DWORD)/*rtc.mday*/7 << 16)
			| ((DWORD)/*rtc.hour*/1 << 11)
			| ((DWORD)/*rtc.min*/26 << 5)
			| ((DWORD)/*rtc.sec*/6 >> 1);
}

//-----------------------------------Константы для сети-----------------------------------------------------------

	uint32_t display_next_update = 0;
	uint32_t loctime;
	uint8_t s, m, h;
	char buf2[5];


//#define NTP_SERVER	inet_addr(62,117,76,142)
#define NTP_SERVER	inet_addr(82,193,117,90)
//91.226.136.136
//62.117.76.142
//82.193.117.90 82,193,117,90	- work 10.02.2020

//#define TIMEZONE	7
#define TIMEZONE	2

static volatile uint16_t ms_count;
static volatile uint32_t second_count;
static volatile uint32_t ntp_next_update;
static volatile uint32_t time_offset;


//int fgColor = 0x00;
//int bgColor = violet;

int dots = 0x00;


//-----------------------------------Прерывания-------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
	if(++ms_count == 1000)
	{
		++second_count;
		ms_count = 0;
		PORTG = PORTG ^ 0x02;
	}
}


void vApplicationIdleHook(){
}


SIGNAL(SIG_ADC)
{	
//	SendToUart0(ADCH);
	ADC_Hlobal = ADCH;
/*
	SendToUart0(ADCH);
//	char str[3] = "000";
//	itoa(ADCH,str,10);

*/

}


//--------------------------------------------------------------------------------------------------------------

void udp_packet(eth_frame_t *frame, uint16_t len)
{
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	uint32_t timestamp;

	if(udp->to_port == NTP_LOCAL_PORT)
	{
		if((timestamp = ntp_parse_reply(udp->data, len)))
		{
			time_offset = timestamp - second_count;
			ntp_next_update = second_count + 12UL*60*60;
		}
	}
}




//------------------------------------Инициализация апаратной части---------------------------------------------
void InitHardware(void)
{

	DDRC = 0x00;
	PORTC = 0xFF;

	DDRD = 0x00;
	PORTD = 0xFF;

	DDRE |= ((1<<PINE3) | (1<<DDE4));
	DDRG |= 1<<PG1;
	DDRB = 0xFF;

	while(1)
	{
		_delay_ms(1000);
		PORTB = 0x00;
		PORTG = 0x00;
		PORTE = 0x00;

		_delay_ms(1000);
		PORTE = PORTE ^ (1<<DDE3);
		PORTE = PORTE ^ (1<<DDE4);
		PORTG = PORTG ^ (1<<DDG1);
		break;
	}

	_delay_ms(1000);
	PORTG = 0x00;
	PORTE = 0x00;




// 	Биты 0,1 порта C будут работать как выходы //
	DDRE |= ((1<<DDE3) | (1<<DDE4));
	DDRG |= 1<<PG1;
	DDRG |= 1<<PING0;


	// init timer, freq = 1 kHz @ CLK = 16 MHz
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS01)|(1<<CS00);
	OCR0A = 187;
	TIMSK0 |= (1<<OCIE0A);
	sei();



	InitUART0(57600,8,1,0,0);		// Настройка УАРТ0
	SendToUart0('0');

	InitUART1(9600,8,1,0,0);		// Настройка УАРТ0
	SendToUart0('1');

	InitUART2(9600,8,1,0,0);		// Настройка УАРТ0
	SendToUart0('2');

	InitUART3(9600,8,1,0,0);		// Настройка УАРТ0
	SendToUart0('3');

	vLcdInit(LS_ULINE);				// Инициализация ЛСД
	SendToUart0('4');

	I2CInit(); 						// Инициализация шины I2C
	SendToUart0('5');

	InitADCEx();					// Иницциализация внешнего АЦП
	SendToUart0('6');			

	transmitStringToUart(0,"\n");
	for (int i = 0; i < 8; i++ ){
		uint16_t res = ReadADCEx(i);
		char buff[50] = "";
		sprintf(buff, "ADC Data = %05d, channel = %d\n", res, i);

		transmitStringToUart(0, buff);
	}

//	lan_init();						// Init LAN
	SendToUart0('7');
}



//-------------------------------------------------------------------------------------------------------------------

int main(void)
{
	_delay_ms(1000);

	InitHardware();


	// Init LCD
	vHd44780Init();
	vHd44780Mode(1,1,0);
	vHd44780Clear();
	vHd44780PutString("StartS1 Hd44780 ");
	//_delay_ms(1000);

	InitSPI(0,0,0,0);
	vHd44780Clear();
	vHd44780PutString("StartS2 InitSPI ");
	//_delay_ms(1000);

	vLs020Init(); 		// Init LS020 LCD
	vHd44780Clear();
	vHd44780PutString("StartS21 Ls020In");
	//_delay_ms(1000);
	
	vLs020Mode8bit(0); 	// Set 8bit LCD host mode
	vHd44780Clear();
	vHd44780PutString("StartS22 Mode8b ");
	//_delay_ms(1000);
	
	vLs020Rectangle(0,0,175,131,bgColor,8);
	vHd44780Clear();
	vHd44780PutString("StartS23 Ls020Rl");
	//_delay_ms(1000);

	vLs020Rotation_2(30, 30, 14, 8, 0);
	vHd44780Clear();
	vHd44780PutString("StartS4 Ls020Rot");
	//_delay_ms(1000);

	//vLs020PutChar8x14(14, 50, 'P', 0x91D4, 0xFFFF, 16, 0);
	//vLs020PutChar8x14(40, 50, 'A', 0x91D4, 0xFFFF, 16, 0);
	vLs020PutString8x14(40, 50, "test", 0x91D4, 0xFFFF, 16, 0);
	vHd44780Clear();
	vHd44780PutString("StartS5 Chars   ");
	//_delay_ms(1000);

	// Init LAN
	lan_init();

	vHd44780Clear();
	vHd44780PutString("StartS6 lan_init");
	transmitStringToUart(0,"StartS6 lan_init\n");

	for (char i = 0; i<=100; i++)
	{
		vLs020StatusBar(10, 10, 112, 12, black, green, black, 16, 1, i-1, i);
		_delay_ms(10);
	}

	while(1)
	{
		lan_poll();
		
		// Time to send NTP request?
		if(second_count >= ntp_next_update)
		{
			if(!ntp_request(NTP_SERVER))
				ntp_next_update = second_count+2;
			else
				ntp_next_update = second_count+60;
		}

		// Time to refresh display?
		if((time_offset) && (second_count >= display_next_update))
		{
			display_next_update = second_count+1;
			
			loctime = time_offset+second_count + 60UL*60*TIMEZONE;
			s = loctime % 60;
			m = (loctime/60)%60;
			h = (loctime/3600)%24;

			char buff[17] = "";
			sprintf(buff, "%02d:%02d:%02d", h,m,s );
			vLcdGotoXY(0,0);
			vHd44780PutString(buff);
			vLs020PutString8x14(40, 50, buff, 0x91D4, 0xFFFF, 16, 0);
		}
	}

	return 0;
}
