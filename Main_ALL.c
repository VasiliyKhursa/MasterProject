/***Configuration SD Card **********

  Device atmega32
  Freq 4.0MHz
  UART: BAUDRATE 19200

*********************/


//***Configuration Extern ADC
                        
/*
Hardware ALD:
	ATmega32 running @ 16MHz
	FUSE HIGH = 0xC9 LOW=0xFF

	MCP320X Connected to SPI Port

	CS PIN of MCP320X Connected to PD7 (Can be chaned by editing spi.h)

	16x2 LCD Module Connected as
		* RS 	-> PD3
		* R/W 	-> PD6
		* E		-> PB4
		* D4	-> PB0
		* D5	-> PB1
		* D6	-> PB2
		* D7	-> PB3

*/
                                   

/*
char hours 		= 12;
char minutes 	= 34;
char seconds 	= 56;

char days   	= 78;
char months 	= 90;
int years		= 1234;

char  voltage1strP [] 	= "00" ;		//	целая часть
char  voltage1strN [] 	= "000" ;		//	дробная часть
char  current1str [] 	= "0.000" ;
char  current1strP [] 	= "000" ;

char  voltage2str [] 	= "00.000" ;
char  current2str [] 	= "0.000" ;

char  hoursstr [] 		= "00";
char  minutesstr [] 	= "00";
char  secondsstr [] 	= "00";

char  daystr [] 		= "00";
char  monthstr [] 		= "00";
char  yearstr [] 		= "0000";






float voltage1 = 12.345;
float current1 = 6.789;
	
float voltage2 = 10.234;
float current2 = 5.678;

float temper 		= 90.123; 
float irradiation 	= 4567.809;


char  temperstr [] 		= "00.000" ;
char  irradiationstr [] = "0000.000" ;


*/











//	StartTime();		// запускаем часы


//	SetTime(22, 23, 00, 0);
//	SetData(2, 18, 12, 13);
//	StoptTime();
//	StartTime();

/*

	current(256, 1);

*/

/*
	dtostrf(temper,2,3,temperstr);
	dtostrf(irradiation,4,3,irradiationstr);

	utoa(hours,hoursstr,10);
	utoa(minutes,minutesstr,10);
	utoa(seconds,secondsstr,10);

	utoa(day,daystr,10);
	utoa(month,monthstr,10);
	itoa(year,yearstr,10);



	data [26+0] = temperstr[0];
	data [26+1] = temperstr[1];
	data [26+3] = temperstr[3];
	data [26+4] = temperstr[4];
	data [26+5] = temperstr[5];

	data [33+0] = irradiationstr[0];
	data [33+1] = irradiationstr[1];
	data [33+2] = irradiationstr[2];
	data [33+3] = irradiationstr[3];
	data [33+5] = irradiationstr[5];
	data [33+6] = irradiationstr[6];
	data [33+7] = irradiationstr[7];

	data [42+0] = hoursstr[0];
	data [42+1] = hoursstr[1];

	data [45+0] = minutesstr[0];
	data [45+1] = minutesstr[1];

	data [48+0] = secondsstr[0];
	data [48+1] = secondsstr[1];

	data [51+0] = daystr[0];
	data [51+1] = daystr[1];

	data [54+0] = monthstr[0];
	data [54+1] = monthstr[1];

	data [57+0] = yearstr[0];
	data [57+1] = yearstr[1];
	data [57+2] = yearstr[2];
	data [57+3] = yearstr[3];
*/


/*

void FATFSData(void)
{


 	xmit("********FatFs: write data demo********");

	//initialize and mount//
	BYTE status = disk_initialize(0);

	sprintf((char*)buf,"initialize return: %X",status);
	xmit(buf);

	f_err_code = f_mount(0, &FATFS_Obj);
	sprintf((char*)buf,"f_mount return: %X",f_err_code);
	xmit(buf);

	FIL fil_obj;

	//open//
	f_err_code = f_open(&fil_obj, "foo.txt", FA_WRITE);
	sprintf((char*)buf,"f_open return: %X",f_err_code);
	xmit(buf);

	//write//
	f_err_code = f_lseek(&fil_obj,fil_obj.fsize);//смещаем указатель на кол-во байт, равное размеру файла. Т.е. переходим в конец файла.
	sprintf((char*)buf,"f_lseek return: %X, size of file: %d",f_err_code,(int)fil_obj.fsize); 
	xmit(buf);

	f_err_code = f_write(&fil_obj,var,strlen(var),&ByteWrite); //записываем в конец файла строку var
	sprintf((char*)buf,"f_write return: %X, byte wrote: %d",f_err_code,ByteWrite); //возвратит FR_OK=0 в случае успеха, также выводим сколько байтов удалось записать
	xmit(buf);



	//close//
	f_close(&fil_obj); //закрываем файл//


    status = disk_initialize(0); //инициализируем карточку
    sprintf((char*)buf,"initialize return: %X",status); //возвратит 0 в случае успеха
    xmit(buf);
 
    f_err_code = f_mount(0, &FATFS_Obj); //монтируем логический раздел
    sprintf((char*)buf,"f_mount return: %X",f_err_code); //возвратит FR_OK=0 в случае успеха
    xmit(buf);
 
    f_err_code = f_open(&fil_obj, "foo.txt", FA_READ); //пытаемся открыть файл
    sprintf((char*)buf,"f_open return: %X",f_err_code); //возвратит FR_OK=0 в случае успеха, FR_NO_FILE=4, если файл не существует...(остальные значения см. в ff.h)
    xmit(buf);
 
    f_err_code = f_read(&fil_obj,var1,20,&ByteRead); //пытаемся читать 20 байтов с начала файла в переменную var
    sprintf((char*)buf,"f_read return: %X, byte read: %d",f_err_code,ByteRead); //возвратит FR_OK=0 в случае успеха, также выводим сколько байтов удалось считать
    xmit(buf);
 
    sprintf((char*)buf,"var: %s",var1);
    xmit(buf);TX_NEWLINE; //выводим в терминал значение буфера
 
    f_close(&fil_obj); //закрываем файл

}

*/






/*

int main(void)
{


	//Display
	LCDWriteStringXY(0,0,"MCP3208 ADC Test");
	LCDWriteStringXY(0,1,"result = ");


//	char str[3] = "000";
//	itoa(ADCH,str,10);
//	itoa(555,str,10);

	

//	LCDWriteIntXY(9,1,5544,5);		



	sei();

	while(1)
	{

		

	

	InitADC(0,1,0,14, 7, 0);

	_delay_ms(1000);
	
	InitADC(0,1,0,13, 7, 0);

	_delay_ms(1000);

//		Read Channel Number 0
//		result=ReadADCEx(0);

//		LCDWriteIntXY(9,1,result,5);
//		_delay_ms(100);
	}


}




*/








/*


int main2(void)
{



	InitUART0(9600,8,1,0,0);	// Настройка УАРТ0	



	


	




while(1)
{
	DallasTemp();

	_delay_ms(1000);

}
}
*/

















/*   LAN

int main()
{
	uint32_t display_next_update = 0;
	uint32_t loctime;
	uint8_t s, m, h;
	char buf[5];

	// Init LAN
	lan_init();

	// Init LCD
	_delay_ms(30);
	hd44780_init();
	hd44780_mode(1,1,0);
	hd44780_clear();

	// init timer, freq = 1 kHz @ CLK = 16 MHz
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS01)|(1<<CS00);
	OCR0A = 250;
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

			hd44780_clear();
			itoa(h,buf,10);
			hd44780_puts(buf);
			hd44780_puts(":");
			itoa(m,buf,10);
			hd44780_puts(buf);
			hd44780_puts(":");
			itoa(s,buf,10);
			hd44780_puts(buf);

		}
	}

	return 0;
}
*/




/*//-----------------------------------------Чистим мєйн
/*
void USART_init()
{
	// Set baud rate
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0A = 0;
	// Enable receiver and transmitter
	UCSR0B = (1<<TXEN0);
	// Set frame format
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) ;
}

void USART0_write(unsigned char data)
{
	while ( !( UCSR0A & (1<<UDRE0)) ) ;
	UDR0 = data;
}








void WriteToTxt(void)
{
	xmit("********FatFs: write data solar********");

	//initialize and mount//
	BYTE status = disk_initialize(0);
	
	status = disk_initialize(0); //инициализируем карточку
    sprintf((char*)buf,"initialize return: %X",status); //возвратит 0 в случае успеха
    xmit(buf);

    f_err_code = f_mount(0, &FATFS_Obj); //монтируем логический раздел
    sprintf((char*)buf,"f_mount return: %X",f_err_code); //возвратит FR_OK=0 в случае успеха
    xmit(buf);

	FIL fil_obj;	

	f_err_code = f_open(&fil_obj, "foo.txt", FA_WRITE); //пытаемся открыть файл
    sprintf((char*)buf,"f_open return: %X",f_err_code); //возвратит FR_OK=0 в случае успеха, FR_NO_FILE=4, если файл не существует...(остальные значения см. в ff.h)
    xmit(buf);

	//write//
	f_err_code = f_lseek(&fil_obj,fil_obj.fsize);//смещаем указатель на кол-во байт, равное размеру файла. Т.е. переходим в конец файла.
	sprintf((char*)buf,"f_lseek return: %X, size of file: %ld",f_err_code,fil_obj.fsize); 
	xmit(buf);


	f_err_code = f_write(&fil_obj,dataFAT,strlen(dataFAT),&ByteWrite); //записываем в конец файла строку var
	sprintf((char*)buf,"f_write return: %X, byte wrote: %d",f_err_code,ByteWrite); //возвратит FR_OK=0 в случае успеха, также выводим сколько байтов удалось записать
	xmit(buf);


	f_close(&fil_obj); //закрываем файл//

}


int main2(void)
{  
	_delay_ms(100); 

	stdout = &usart_str; 		// указываем, куда будет выводить printf 
	
	unsigned int result;
//	DDRK = 0x00;
//	PORTK = 0xFF;	
//	DDRJ = 0x00;
//	PORTJ = 0xFF;

	
	InitHardware();	
	

	LCDWriteStringXY(0,1,"Date    .  .20  ");
	LCDWriteStringXY(0,0,"Time    :  :    ");


	sei();


//	result=ReadADCEx(0);

//	LCDWriteIntXY(9,1,result,5);

// 	WriteToTxt();
	unsigned int A1 = 0, U1 = 0, A2 = 0, U2 = 0;
	unsigned char a = 0;
	
	while(1)
	{ 
	
		if (a == 60)
		{
		
		a = 0;
/*

		current(A1/60, 0);
		_delay_ms(100);


		voltage(U1/60, 0);
		_delay_ms(100);


		current(A2/60, 1);
		_delay_ms(100);


		voltage(U2/60, 1);
		_delay_ms(100);
*/
			
/*
		result=ReadADCEx(0);
		_delay_ms(100);

		voltage_ext(result, 1);
		_delay_ms(100);

		result=ReadADCEx(1);
		_delay_ms(100);
		current_ext(result, 1);
		_delay_ms(100);

		result=ReadADCEx(2);
		_delay_ms(100);
		voltage_ext(result, 0);
		_delay_ms(100);

		result=ReadADCEx(7);
		_delay_ms(100);
		current_ext(result, 0);
		_delay_ms(100);

*/		
		vDS1307TimeReadWriteLCD();
		vDS1307DateReadWriteLCD();
//		ConvertDataAndTime();
/*
		DallasTemp();

		WriteToTxt();
		xmit(dataFAT);
		

		A1 = 0; 
		U1 = 0;
		A2 = 0; 
		U2 = 0;	
		}

		else 
		{
		InitADC(0,1,0,15, 7, 0);
		_delay_ms(100);
		A1 = A1 + (ADC_Hlobal - 4);
		

		InitADC(0,1,0,14, 7, 0);
		_delay_ms(100);
		U1 = U1 + (ADC_Hlobal);


		InitADC(0,1,0,13, 7, 0);
		_delay_ms(100);
		A2 = A2 + (ADC_Hlobal - 4);


		InitADC(0,1,0,12, 7, 0);
		_delay_ms(100);
		U2 = U2 + (ADC_Hlobal);


		a++;
		_delay_ms(575);
		}


	}
} 









*/

