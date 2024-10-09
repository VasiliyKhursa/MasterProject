#ifndef AT_COMMAND_H
#define AT_COMMAND_H

#define		R_N					SendToUart(0x0D);	SendToUart(0x0A);
#define		mode0				SendToUart('=');	SendToUart('?');
#define		mode1				SendToUart('=');	
#define		mode2				SendToUart('?');

#define 	AT_CMD 			AT()
#define 	AT_CGSN_CMD 	AT_CGSN()
#define 	IMEI_CMD 		AT_CGSN()
#define 	AT_CPAS_CMD 	AT_CPAS()
#define 	ATA_CMD 		ATA()
#define 	AT_CHUP_CMD 	AT_CHUP()
#define 	AT_CLCC_CMD 	AT_CLCC()
#define 	AT_CBC_CMD 		AT_CBC()
//#define 	ATD_CMD 		ATD()
#define 	AT_CGATT_CMD 	AT_CGATT()
#define 	AT_CEER_CMD 	AT_CEER()
#define 	AT_V_CMD 		AT_V()
#define 	AT_CPAS_CMD 	AT_CPAS()







//--------------------------------------Основные------------------------------------------------
void AT_CGMI(void);		// Данная команда идентифицирует производителя.
void AT_CGMM(void);		// Данная команда используется для получения поддерживаемых полос частот.
void AT_CGMR(void);		// Данная команда используется для получения данных о версии программного обеспечения.
void AT_CGSN(void);		// Позволяет получить ИМЕЙ
void AT_CSCS(void);		// Выбор набора символов терминального оборудования








void AT(void);

void AT_CPAS(void);
void ATA(void);
void AT_CHUP(void);
void AT_CLCC(void);
void AT_CBC(void);
void ATD(void);
void AT_V(void);
void AT_CEER(void);
void AT_CGATT(void);




void AT_CBST(void);
void AT_CBST_1(void);
void AT_CBST_2(void);



void AT_CMGS(void);
void AT_CMGF(void);




//---------------

void AT_CGDCONT(void);
void AT_CIPCSGP(void);



void AT_CMEE(void);
//void AT_WIND(void);
void AT_CGQREQ(void);
void AT_CGQMIN(void);
void AT_CGACT(void);
void AT_CGDATA(void);
void AT_CGCLASS(void);
void AT_CGREG(void);
void TEST(void);

void AT_CTESTGPRS(void);


void AT_UNIVERSAL(char mode, const char * text, const char * setup);
void AT_UNIVERSAL_P(char mode, const char * text, const char * setup);
#endif
