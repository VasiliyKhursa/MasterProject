#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "AT_COMMAND.h"
#include "uart.h"

#define SendToUart(x)	SendToUart2(x)


const char numb1[] 		PROGMEM		= "0634257125";
const char numb2[] 		PROGMEM		= "*111#";
const char numb_gprs1[] PROGMEM		= "*99#";
const char numb_gprs2[] PROGMEM		= "*99***1#";


const char cmgs[] 		PROGMEM		= "AT+CMGS=50";
const char sms[]  		PROGMEM		= "0011000C918360435217520008FF240422043504410442043E0432043E043500200441043E043E043104490435043D04380435";


//--------------------------------------Основные------------------------------------------------
const char at_cgmi[] 	PROGMEM		= "AT+CGMI";		// Данная команда идентифицирует производителя.
const char at_cgmm[] 	PROGMEM		= "AT+CGMM";		// Данная команда используется для получения поддерживаемых полос частот. 
const char at_cgmr[] 	PROGMEM		= "AT+CGMR";		// Данная команда используется для получения данных о версии программного обеспечения. 
const char at_cgsn[] 	PROGMEM		= "AT+CGSN";		// Позволяет получить ИМЕЙ
const char at_cscs[] 	PROGMEM		= "AT+CSCS";		// Выбор набора символов терминального оборудования: +CSCS									//НЕРАЬБОТАЕТ



//--------------------------------------общие

const char at[]   		PROGMEM		= "AT";
const char at_cbc[] 	PROGMEM 	= "AT+CBC";
const char at_ceer[]	PROGMEM 	= "AT+CEER";
const char at_v[] 		PROGMEM		= "AT&V+CBST";


const char at_cbst[] 	PROGMEM		= "AT+CBST";
const char cbstt[] 		PROGMEM		= "AT+CBST=71,0,1";








//--------------------------------------телефон
const char ata[]  		PROGMEM		= "ATA";
const char at_chup[]	PROGMEM 	= "AT+CHUP";

const char at_cpas[] 	PROGMEM		= "AT+CPAS";
const char at_clcc[] 	PROGMEM		= "AT+CLCC";
const char atd[]  		PROGMEM		= "ATD";

//--------------------------------------GPRS
const char at_cgatt[]  			PROGMEM		= "AT+CGATT";

const char at_cgdcont[]  		PROGMEM		= "AT+CGDCONT";
const char at_cgdcont_gprs[]	PROGMEM		= "AT+CGDCONT=1,IP,internet";
const char at_cgqreq[] 			PROGMEM		= "AT+CGQREQ";
const char at_cgqmin[]  		PROGMEM		= "AT+CGQMIN";
const char at_cgact[]  			PROGMEM		= "AT+CGACT";
const char at_cgdata[]  		PROGMEM		= "AT+CGDATA";
const char at_cgclass[]  		PROGMEM		= "AT+CGCLASS";
const char at_cgreg[]  			PROGMEM		= "AT+CGREG";
const char at_cipcsgp[]			PROGMEM		= "AT+CIPCSGP";
const char at_cmgf[] 			PROGMEM		= "AT+CMGF";
const char at_cmee[] 			PROGMEM		= "AT+CMEE";
const char at_wind[] 			PROGMEM		= "AT+WIND";  




//----------------------------------------------------------------------------------
// Универсальная функция отправки команд из памяти данных
// mode 0 - text + "=?"			- возможные настройки для данной команды
// mode 1 - text + "=" + setup	- настроить модем
// mode 2 - text + "?"			- текущие настройки модема
// mode 3 - text				- просто команда без настроек

void AT_UNIVERSAL(char mode, const char * text, const char * setup)
{
	unsigned char i = 0;
	while (text[i] != 0)
		{
		SendToUart(text[i]);
		i++;
		}
	i = 0;
	switch(mode)
		{
		case 0 : 	mode0; 	break;
		case 1 : 	mode1; 	
					while (setup[i] != 0)
							{	
							SendToUart(setup[i]);
							i++;
							}
							break;
		case 2 : 	mode2; 	break;
		case 3 : 			break;
		}

	R_N;

}
//----------------------------------------------------------------------------------
// Универсальная функция отправки команд из памяти программ
// mode 0 - text + "=?"			- возможные настройки для данной команды
// mode 1 - text + "=" + setup	- настроить модем
// mode 2 - text + "?"			- текущие настройки модема
// mode 3 - text				- просто команда без настроек

void AT_UNIVERSAL_P(char mode, const char * text, const char * setup)
{
	char i = 0;
	while (pgm_read_byte(text + i) != 0)
		{
		SendToUart(pgm_read_byte (text + i));
		i++;
		}
	i = 0;
	switch(mode)
		{
		case 0 : 	mode0; 	break;
		case 1 : 	mode1; 	
					while (pgm_read_byte(setup + i) != 0)
							{	
							SendToUart(pgm_read_byte (setup + i));
							i++;
							}
							break;
		case 2 : 	mode2; 	break;
		case 3 : 			break;
		}

	R_N;

}
//----------------------------------------------------------------------------------
// тестовая команда для проверки возможных команд
void TEST(void)
{
for (char i = 0; i < 26; i++)
	{
	for (char j = 0; j < 26; j++)
		{
		for (char k = 0; k < 26; k++)
			{
			for (char l = 0; l < 26; l++)
				{
				SendToUart('A');
				SendToUart('T');
				SendToUart('+');
				SendToUart(i + 65);
				SendToUart(j + 65);
				SendToUart(k + 65);
				SendToUart(l + 65);
				SendToUart('=');
				SendToUart('?');
				SendToUart(0x0D);	
				SendToUart(0x0A);
				_delay_ms(50);				
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
//	Тестовая команда
void AT(void)
{
	AT_UNIVERSAL_P(3, at, " ");
}

//----------------------------------------------------------------------------------
// Данная команда идентифицирует производителя.

void AT_CGMI(void)
{
	AT_UNIVERSAL_P(3, at_cgmi, " ");
}

//----------------------------------------------------------------------------------
// Данная команда используется для получения поддерживаемых полос частот. 

void AT_CGMM(void)
{
	AT_UNIVERSAL_P(3, at_cgmm, " ");
}

//----------------------------------------------------------------------------------
// Данная команда используется для получения данных о версии программного обеспечения.

void AT_CGMR(void)
{
	AT_UNIVERSAL_P(3, at_cgmr, " ");
}

//----------------------------------------------------------------------------------
// Позволяет получить ИМЕЙ

void AT_CGSN(void)
{
	AT_UNIVERSAL_P(3, at_cgsn, " ");
}
//----------------------------------------------------------------------------------
// Выбор набора символов терминального оборудования
// +CSCS: ("GSM","UCS2")
// GSM - алфавит GSM по умолчанию 
void AT_CSCS(void)
{
	AT_UNIVERSAL_P(0, at_cscs, " ");
}














//----------------------------------------------------------------------------------
// запрос уровня заряда батареи телефона
// ответ в виде:
// +CBC: х где х уровень заряда, умножив на 100 можно получить эту величину в процентах.

void AT_CBC(void)
{
	AT_UNIVERSAL_P(3, at_cbc, " ");
}

//----------------------------------------------------------------------------------
// состояние телефона в определённый момент времени
// 0 – Телефон в режиме ожидания звонка, 
// 3 – входящий звонок, 
// 4 – идёт разговор.

void AT_CPAS(void)
{
	AT_UNIVERSAL_P(3, at_cpas, " ");
}

//----------------------------------------------------------------------------------
// Данная команда используется для отображения ошибки модема. 
// 

void AT_CEER(void)
{
	AT_UNIVERSAL_P(3, at_ceer, " ");
}

//----------------------------------------------------------------------------------
// включить отчеты об ошибках мобильного устройства
// 

void AT_CMEE(void)
{
	AT_UNIVERSAL_P(3, at_cmee, " ");
}

//----------------------------------------------------------------------------------
// запросить отображение общей индикации 
// 

void AT_WIND(void)
{
	AT_UNIVERSAL_P(3, at_wind, " ");
}

//----------------------------------------------------------------------------------
// Данная команда используется для отображения настроек модема. 
// 

void AT_V(void)
{
	AT_UNIVERSAL_P(3, at_v, " ");
}

//----------------------------------------------------------------------------------
// Принять входящий звонок
void ATA(void)
{
	AT_UNIVERSAL_P(3, ata, " ");
}

//----------------------------------------------------------------------------------
// Отклонить входящий вызов
void AT_CHUP(void)
{
	AT_UNIVERSAL_P(3, at_chup, " ");
}


//----------------------------------------------------------------------------------
// Позволяет узнать номер входящего вызова
void AT_CLCC(void)
{
	AT_UNIVERSAL_P(3, at_clcc, " ");
}

//----------------------------------------------------------------------------------
// Заставляет телефон набрать номер x. 
// Если звонок не состоялся (например нет сигнала базовой станции) 
// то телефон отвечает NO DIALTONE. 
void ATD(void)
{
	for(uint8_t n=0; n<strlen_P(atd) ; n++)
		SendToUart(	pgm_read_byte (atd + n));


	for(uint8_t n=0; n<strlen_P(numb1) ; n++)
		SendToUart(	pgm_read_byte (numb1 + n));
	
	R_N;
}

//----------------------------------------------------------------------------------
// Команда отправляющая СМС
// После отправки этой команды нужно передать символ перевода строки.
// Теперь можно передавать непосредственно сообщение.  
// Сообщение должно заканчиваться символом с кодом 0x1A.
// 
void AT_CMGS(void)
{
	for(uint8_t n=0; n<strlen_P(cmgs) ; n++)
		SendToUart(pgm_read_byte (cmgs + n));
	
	R_N;

	_delay_ms(500);	

	for(uint8_t n=0; n<strlen_P(sms)  ; n++)
		SendToUart(pgm_read_byte (sms + n));

	SendToUart(26);	//"$ "
		
	R_N;

}


//----------------------------------------------------------------------------------
// Команда теста СМС
// 
void AT_CMGF(void)
{
	AT_UNIVERSAL_P(3, at_cmgf, " ");
}

//----------------------------------------------------------------------------------
// Данная команда используется для отображения настроек модема. 
// 

void AT_CBST(void)
{
	AT_UNIVERSAL_P(3, at_cbst, " ");
}


//-----------------------------------------------------------------------------------
// AT+CGATT=1 - команда регистрации в сети GPRS.
// Вернет ОК в случае успешной регистрации
void AT_CGATT(void)
{
	AT_UNIVERSAL_P(3, at_cgatt, " ");
}

 
// 
void AT_CGDCONT(void)
{
	AT_UNIVERSAL_P(3, at_cgdcont, " ");
}

void AT_CIPCSGP(void)
{
	AT_UNIVERSAL_P(3, at_cipcsgp, " ");
}


void AT_CGQREQ(void)
{
	AT_UNIVERSAL_P(3, at_cgqreq, " ");
}

void AT_CGQMIN(void)
{
	AT_UNIVERSAL(3, at_cgqmin, " ");
}

void AT_CGACT(void)
{
	AT_UNIVERSAL_P(3, at_cgact, " ");
}

void AT_CGDATA(void)
{
	AT_UNIVERSAL_P(3, at_cgdata, " ");
}

void AT_CGCLASS(void)
{
	AT_UNIVERSAL_P(3, at_cgclass, " ");
}

void AT_CGREG(void)
{

	AT_UNIVERSAL_P(3, at_cgreg, " ");
}



//const char gprs_test1[] = "AT#GPRSMODE=0";
/*
void AT_CTESTGPRS(void)
{
	AT_UNIVERSAL(3, at_gprs, " ");
}
*/



