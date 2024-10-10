/**
 **************************************************************************************************
 *  @file           : main.c
 *  @brief          :
 **************************************************************************************************
 *  @author     Vasyl Khursa
 *
 *  @description:
 *
 *  @section  HISTORY
 *  v1.0  - First version
 *
 *  @copyright  (c) 2024
 *
 **************************************************************************************************
 */

//---------------------------------------Info--------------------------------------//
/*
	Device 			-	Atmega2560
  	Extern ADC		-	MCP320X
	LCD				-	1602
	LCD				-	LS020
	Ethenet			- 	ENC28J60
	RTC				-	DS1307
	Temp			-	DS18B20
	Freq 			- 	12.0MHz
	RTOS			-	FreeRTOS v 8.0.0

//���������� 
 	
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

							
	LS020(UART1):
		* CS_LS020		-> PB5
		* Reset			-> PB6
		* RS			-> PB7


	ENC28J60:
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
							
	LCD1602:
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

U2	Channel_0		Channel_12		 PK4		// ���������� 2
A2	Channel_1		Channel_13		 PK5		// ��� 2
U1	Channel_2		Channel_14		 PK6		// ���������� 1
A1	Channel_3		Channel_15		 PK7		// ��� 1



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

//-----------------------------------��������� ��� �������� �������------------------------------------------------

char dataFAT []  = "30.299	2.001	29.311	4.999	35.125	1000.999	12:40:13	12.10.1991\n\r";		// ������ ������ � ������

unsigned char hour, minute, second, date, month;									// ���������� ���������� ������� � ����
unsigned int  year;																	// ���������� ���������� ��� �������� ���� (1991)

unsigned char temp;

float VoltagePower = 4.7;															// ���������� ���������� ��� ���������� ������� ��� � �������� ����������

unsigned char ADC_Hlobal;															// ���������� ���������� ��� �������� ������ � ���

FILE usart_str = FDEV_SETUP_STREAM(SendToUart0, NULL, _FDEV_SETUP_WRITE); 			// ��� ������� printf
#define xmit(a) {transmitString((unsigned char*)a);TX_NEWLINE;}


char buf[100];
static FRESULT f_err_code;
static FATFS FATFS_Obj;
UINT ByteWrite = 0;

char* var="������";
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

//-----------------------------------��������� ��� ����-----------------------------------------------------------

	uint32_t display_next_update = 0;
	uint32_t loctime;
	uint8_t s, m, h;
	char buf2[5];


//#define NTP_SERVER	inet_addr(62,117,76,142)
#define NTP_SERVER	inet_addr(91,226,136,136)
//#define TIMEZONE	7
#define TIMEZONE	3

static volatile uint16_t ms_count;
static volatile uint32_t second_count;
static volatile uint32_t ntp_next_update;
static volatile uint32_t time_offset;


//int fgColor = 0x00;
//int bgColor = violet;

int dots = 0x00;






//-----------------------------------����������-------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{

	if(++ms_count == 1000)
	{
		++second_count;
		ms_count = 0;

/*		
		second++;
		if(second >= 60) 
			{
			second = 0;
			minute++;

			if (minute >=60)
				{
				minute = 0;
				hour++;
				}
			}

	vLcdWriteTime();
	vLcdWriteDate();
*/
	}


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




//------------------------------------������������� ��������� �����---------------------------------------------
void InitHardware(void)
{


	DDRC = 0x00;
	PORTC = 0xFF;

	DDRD = 0x00;
	PORTD = 0xFF;


// 	���� 0,1 ����� C ����� �������� ��� ������ //
	DDRE |= ((1<<DDE3) | (1<<DDE4));
	DDRG |= 1<<PG1;
	DDRG |= 1<<PING0;


	// init timer, freq = 1 kHz @ CLK = 16 MHz
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS01)|(1<<CS00);
	OCR0A = 187;
	TIMSK0 |= (1<<OCIE0A);
	sei();



	InitUART0(9600,8,1,0,0);		// ��������� ����0

	InitUART2(9600,8,1,0,0);		// ��������� ����0
	SendToUart2(0x11);

	vLcdInit(LS_ULINE);				// ������������� ���
	SendToUart2(0x22);

	I2CInit(); 						// ������������� ���� I2C
	SendToUart2(0x33);

	InitADCEx();					// �������������� �������� ���
	SendToUart2(0x44);			

	lan_init();						// Init LAN
}



//-------------------------------------------------------------------------------------------------------------------

int main(void)
{

	// Init LAN
	lan_init();

	// Init LCD
	_delay_ms(30);
	vHd44780Init();
	vHd44780Mode(1,1,0);
	vHd44780Clear();

	// init timer, freq = 1 kHz @ CLK = 16 MHz
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS01)|(1<<CS00);
	OCR0A = 187;
	TIMSK0 |= (1<<OCIE0A);
	sei();


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

			vHd44780Clear();
			itoa(h,buf2,10);
			vHd44780PutString(buf2);
			vHd44780PutString(":");
			itoa(m,buf2,10);
			vHd44780PutString(buf2);
			vHd44780PutString(":");
			itoa(s,buf2,10);
			vHd44780PutString(buf2);

		}
	}

	return 0;
}




int main3 (void)
	{

	InitSPI(0,0,0,0);

	vLs020Init(); 		// Init LS020 LCD
	vLs020Mode8bit(0); 	// Set 8bit LCD host mode
	
	vLs020Rectangle(0,0,175,131,bgColor,8);


	vLs020Rotation_2(30, 30, 14, 8, 0);

	vLs020PutChar8x14(14, 50, 0x50, 0x91D4, 0xFFFF, 16, 0);


	
	while(1);
	return 1;

	
}







































//-------------------------------------------------------------------------------------------------------------------
//---------------------------------//
//            ������ #0            //
//---------------------------------//
void vApplicationIdleHook(void)
{

  	PORTG ^= (1 << PG1);
	_delay_ms(50);

}




//---------------------------------//
//            ������ #1            //
//---------------------------------//
void vTask1( void *pvParameters )
{
	unsigned short *usMainTask1Delay;
	usMainTask1Delay = (unsigned short *) pvParameters;
	
	// ������ ��������� �� ���� ������������ ����� //
	for(;;)
	{
	// ������������� ��������� ���� PE4 //
	PORTE ^= (1 << PE4);
	// �������� �� ��������� ������ �1 //
 	vTaskDelay(*usMainTask1Delay); 							// T1 = 250 ��������� �����
 	}
// ���������� ������ ���� ��������� ����� �� ������������ ����� //
vTaskDelete(NULL);
}

//---------------------------------//
//            ������ #2            //
//---------------------------------//
void vTask2( void *pvParameters )
{
	unsigned short *usMainTask2Delay;
	usMainTask2Delay = (unsigned short*) pvParameters; 
	for(;;)
 	{
	// ������������� ��������� ���� PE3 //
  	PORTE ^= (1 << PE3);
	// �������� �� ��������� ������ �2 //
	vTaskDelay(*usMainTask2Delay);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}


//---------------------------------//
//            ������ #3            //
//---------------------------------//
void vTask3( void *pvParameters )
{
	unsigned short *usMainTask3Delay;
	usMainTask3Delay = (unsigned short *) pvParameters;
	for(;;)
 	{
	// ������������� ��������� ���� PE3 //
  	PORTG ^= (1 << PG1);
	// �������� �� ��������� ������ �2 //
	vTaskDelay(*usMainTask3Delay);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}


//---------------------------------//
//            ������ #4            //
//---------------------------------//
void vTask4( void *pvParameters )
{
	for(;;)
 	{
	taskENTER_CRITICAL();
	vDS1307TimeReadWriteLCD();
	vDS1307DateReadWriteLCD();
	taskEXIT_CRITICAL();
	vTaskDelay(50);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}



//---------------------------------//
//            ������ #4            //
//---------------------------------//
void vTask5( void *pvParameters )
{
	for(;;)
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

			vHd44780Clear();
			itoa(h,buf,10);
			vHd44780PutString(buf);
			vHd44780PutString(":");
			itoa(m,buf,10);
			vHd44780PutString(buf);
			vHd44780PutString(":");
			itoa(s,buf,10);
			vHd44780PutString(buf);

		}
	// �������� �� ��������� ������ �2 //
//	vTaskDelay(60000);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}




//---------------------------------//
//            ������ #3            //
//---------------------------------//
void vTask6( void *pvParameters )
{
	for(;;)
 	{
		loctime = time_offset+second_count + 60UL*60*TIMEZONE;
		s = loctime % 60;
		m = (loctime/60)%60;
		h = (loctime/3600)%24;

		vDS1307TimeReadWriteLCD();
/*
//		vHd44780Clear();
		itoa(h,buf,10);
		
		LCDWriteStringXY(7,0,buf);
		LCDWriteStringXY(9,0,":");
		vH44780PutString(buf);

		vH44780PutString(":");
		itoa(m,buf,10);
		LCDWriteStringXY(10,0,buf);
		LCDWriteStringXY(11,0,":");

		vHd44780PutString(buf);

		vHd44780PutString(":");
		itoa(s,buf,10);
		vHd44780PutString(buf);
*/
		vTaskDelay(300);	// �2 = 500 ��������� �����	
	} 					
vTaskDelete(NULL);
}


//---------------------------------//
//            ������ #6            //
//---------------------------------//
void vTask7( void *pvParameters )
{
	for(;;)
 	{
	taskENTER_CRITICAL();
	vDS1307TimeReadWriteLCD();
	vDS1307DateReadWriteLCD();
	taskEXIT_CRITICAL();
	vTaskDelay(50);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}

//---------------------------------//
//            ������ #7            //
//---------------------------------//
void vTask8( void *pvParameters )
{
	for(;;)
 	{
	taskENTER_CRITICAL();
	xmit("********FatFs: write data solar********");

	//initialize and mount//
	BYTE status = disk_initialize(0);
	
	status = disk_initialize(0); //�������������� ��������
    sprintf((char*)buf,"initialize return: %X",status); //��������� 0 � ������ ������
    xmit(buf);

    f_err_code = f_mount(0, &FATFS_Obj); //��������� ���������� ������
    sprintf((char*)buf,"f_mount return: %X",f_err_code); //��������� FR_OK=0 � ������ ������
    xmit(buf);

	FIL fil_obj;	

	f_err_code = f_open(&fil_obj, "foo.txt", FA_WRITE); //�������� ������� ����
    sprintf((char*)buf,"f_open return: %X",f_err_code); //��������� FR_OK=0 � ������ ������, FR_NO_FILE=4, ���� ���� �� ����������...(��������� �������� ��. � ff.h)
    xmit(buf);

	//write//
	f_err_code = f_lseek(&fil_obj,fil_obj.fsize);//������� ��������� �� ���-�� ����, ������ ������� �����. �.�. ��������� � ����� �����.
	sprintf((char*)buf,"f_lseek return: %X, size of file: %ld",f_err_code,fil_obj.fsize); 
	xmit(buf);


	f_err_code = f_write(&fil_obj,dataFAT,strlen(dataFAT),&ByteWrite); //���������� � ����� ����� ������ var
	sprintf((char*)buf,"f_write return: %X, byte wrote: %d",f_err_code,ByteWrite); //��������� FR_OK=0 � ������ ������, ����� ������� ������� ������ ������� ��������
	xmit(buf);


	f_close(&fil_obj); //��������� ����//
	taskEXIT_CRITICAL();

	vTaskDelay(1000);
	} 					// �2 = 500 ��������� �����	}
vTaskDelete(NULL);
}




//---------------------------------//
//           Entry Point           //
//---------------------------------//
int main5( void )
{

// 	���� 0,1 ����� C ����� �������� ��� ������ //
	DDRE |= ((1<<DDE3) | (1<<DDE4));
	DDRG |= 1<<PG1;
	DDRG |= 1<<PING0;

	unsigned short usMainTask1Delay = 1000;
	unsigned short usMainTask2Delay = 500;
	unsigned short usMainTask3Delay = 250;

	unsigned short *pointer;

	void *pointervoid;

	
	
	pointer =& usMainTask1Delay;

	pointervoid = (void *) pointer;




	InitHardware();	// ����������� ���������
	vLcdWriteStringXY(0,0,"Time    :  :    ");
	vLcdWriteStringXY(0,1,"Date    .  .20  ");




//	vDS1307SetTime(20, 14, 00, 0);
//	vDS1307SetData(3, 18, 05, 15);
	vDS1307StartTime();		// ��������� ����


	while(1)
	{ 

	vDS1307TimeReadWriteLCD();
	vDS1307DateReadWriteLCD();	
	_delay_ms(50);
	}



//��������� ������ #1. ��� ������ ������� xTaskCreate
//��������� ������ ��� �� ������������, ���������������� ����������.
//� ���� ������� �� ������������ �������� �� ���������� �������� ������ 
	xTaskCreate ( vTask1, 						// ��������� �� ����������� ������� 
             	(const char *) "Task1", 		// ��� ������, ����������� ��� ������� FreeRTOS //
             	configMINIMAL_STACK_SIZE,  		// ������ ����� ������ //
             	(void*)&usMainTask1Delay,		// ������������ �������� //
             	1,  							// ��������� ������ //
             	NULL );  						// ���������� ������ //


// ��������� ������ #2 //
	xTaskCreate ( vTask2, 						
             	(const char * ) "Task2", 		
             	configMINIMAL_STACK_SIZE, 		
             	(void*)&usMainTask2Delay, 							
             	1, 							
             	NULL ); 						
/*
// ��������� ������ #3 //
	xTaskCreate ( vTask3, 						
             	(const char * ) "Task3", 		
             	configMINIMAL_STACK_SIZE, 		
             	(void*)&usMainTask3Delay, 							
             	1, 							
             	NULL ); 
*/


















/*
// ��������� ������ #4 //
	xTaskCreate ( vTask4, 						
             	(const char * ) "Task4", 		
             	configMINIMAL_STACK_SIZE, 		
             	NULL, 							
             	3, 							
             	NULL ); 



// ��������� ������ #6 //
	xTaskCreate ( vTask6, 						
             	(const char * ) "Task6", 		
             	configMINIMAL_STACK_SIZE, 		
             	NULL, 							
             	1, 							
             	NULL );

// ��������� ������ #7 //
	xTaskCreate ( vTask7, 						
             	(const char * ) "Task7", 		
             	configMINIMAL_STACK_SIZE, 		
             	NULL, 							
             	1, 							
             	NULL );
*/

	vTaskStartScheduler();
	return 0;
}
//---------------------------------//
//           End of File           //
//---------------------------------//




int main4(void)
{  
	_delay_ms(100); 
	
	InitHardware();	
	
//	vDS1307SetTime(16, 13, 00, 0);
//	vDS1307SetData(03, 26, 02, 00);

//	vDS1307StartTime();

	vLcdWriteStringXY(0,1,"Date    .  .20  ");
	vLcdWriteStringXY(0,0,"Time    :  :    ");


//	vDS1307TimeReadWriteLCD();
//	vDS1307DateReadWriteLCD();


	while(1)

	{
/*
	vLcdWriteTime();
	vLcdWriteDate();

	second++;
	_delay_ms(1000);
*/	
	}

	return 0;
}
