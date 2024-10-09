#include "adc.h"
#include <avr/io.h>


void InitADC(char Mode, char VolSel, char ModeInput, char Channel, char Prescaler, char AutoTrig)
{

//Mode1: selects start source
//	0  -  Free Running mode
//	1  -  Analog Comparator
//	2  -  External Interrupt Request 0
//	3  -  Timer/Counter0 Compare Match
//	4  -  Timer/Counter0 Overflow
//	5  -  Timer/Counter Compare Match B
//	6  -  Timer/Counter1 Overflow
//	7  -  Timer/Counter1 Capture Event

//VolSel: Voltage Reference Selection
//	0  -  AREF, Internal Vref turned off
//	1  -  AVCC with external capacitor at AREF pin
//	2  -  Internal 1.1V Voltage Reference with external capacitor at AREF pin
//	3  -  Internal 2.56V Voltage Reference with external capacitor at AREF pin

//ModeInput: gain for the differential channels
//	0  -  Single Ended Input
//	1  -  Differential Input x10 or x200
//	2  -  Differential Input x1
//	3  -  Differential Input x1 and ADC to GND and internal soure 1.22V

//Channel
//	0  -  ADC0
//	1  -  ADC1
//	2  -  ADC2
//	3  -  ADC3
//	4  -  ADC4
//	5  -  ADC5
//	6  -  ADC6
//	7  -  ADC7
//	8  -  ADC8
//	9  -  ADC9
//	10  -  ADC10
//	11  -  ADC11
//	12  -  ADC12
//	13  -  ADC13
//	14  -  ADC14
//	15  -  ADC15

//Prescaler
//	0  -  Fadc=Fosc/2
//	1  -  Fadc=Fosc/2
//	2  -  Fadc=Fosc/4
//	3  -  Fadc=Fosc/8
//	4  -  Fadc=Fosc/16
//	5  -  Fadc=Fosc/32
//	6  -  Fadc=Fosc/64
//	7  -  Fadc=Fosc/128

//AutoTrig: ADC Auto Trigger Enable
//	0  -  Auto Trigger Disable
//	1  -  Auto Trigger Enable
	
	ADMUX = 0x00;			// обнул€ем


//ADMUX:ADC Multiplexer Selection Register Ц ADMUX 
// Bit 7:6 	Ц REFS1:0 	: Reference Selection Bits
// Bit 5 	Ц ADLAR 		: ADC Left Adjust Result
// Bits 4:0 Ц MUX4:0 	: Analog Channel and Gain Selection Bits
 
 	ADCSRA = 0x00;			// обнул€ем

//ADCSRA:ADC Control and Status Register A Ц ADCSRA 
// Bit 7 	Ц ADEN 			: ADC Enable  
// Bit 6 	Ц ADSC 			: ADC Start Conversion 
// Bit 5 	Ц ADATE 	 		: ADC Auto Trigger Enable 
// Bit 4 	Ц ADIF 			: ADC Interrupt Flag 
// Bit 3 	Ц ADIE 			: ADC Interrupt Enable  
// Bits 2:0 Ц ADPS2:0 		: ADC Prescaler Select Bits 
	
	ADCH = 0x00;			// обнул€ем
	ADCL = 0x00;			// обнул€ем


//The ADC Data Register Ц ADCL and ADCH 
	
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));			// обнул€ем
		
//SFIOR: Special FunctionIO Register Ц SFIOR 
// Bit 7:5 	Ц ADTS2:0 		: ADC Auto Trigger Source 
// Bit 4 	Ц ADHSM: ADC High Speed Mode         		- BIT DETLETED !!!

switch (Mode) 
	{ 
		case 0: ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));											break;
	  	case 1:	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1));				ADCSRA |=   1<<ADTS0;					break;
	  	case 2: ADCSRB &= ~((1<<ADTS2)|(1<<ADTS0));				ADCSRA |=   1<<ADTS1;					break;
		case 3: ADCSRB &= ~(1<<ADTS2);							ADCSRA |= ((1<<ADTS1)|(1<<ADTS0));		break;	
		case 4: ADCSRB &= ~((1<<ADTS1)|(1<<ADTS0));				ADCSRA |=   1<<ADTS2;					break;
	  	case 5:	ADCSRB &= ~(1<<ADTS1);							ADCSRA |= ((1<<ADTS2)|(1<<ADTS0));		break;
	  	case 6: ADCSRB &= ~(1<<ADTS0);							ADCSRA |= ((1<<ADTS2)|(1<<ADTS1));		break;
		case 7: ADCSRB |=  ((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));											break;;	
	}

switch (VolSel) 
	{ 
		case 0: ADMUX &= ~((1<<REFS1)|(1<<REFS0));						break;
	  	case 1:	ADMUX &=  ~(1<<REFS1);				ADMUX |= 1<<REFS0; 	break;
	  	case 2: ADMUX &=  ~(1<<REFS0);				ADMUX |= 1<<REFS1; 	break;
		case 3: ADMUX |=  ((1<<REFS1)|(1<<REFS0));						break;	
	}

switch (Channel)  
	{ 
		case 0: ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));												break;
	  	case 1:	ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~((1<<MUX2)|(1<<MUX1));				ADMUX |=   1<<MUX0;						break;
	  	case 2: ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~((1<<MUX2)|(1<<MUX0));				ADMUX |=   1<<MUX1;						break;
		case 3: ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~(1<<MUX2);							ADMUX |= ((1<<MUX1)|(1<<MUX0));			break;	
		case 4: ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~((1<<MUX1)|(1<<MUX0));				ADMUX |=   1<<MUX2;						break;
	  	case 5:	ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~(1<<MUX1);							ADMUX |= ((1<<MUX2)|(1<<MUX0));			break;
	  	case 6: ADCSRB &=  ~(1<<MUX5); 			ADMUX &= ~(1<<MUX0);							ADMUX |= ((1<<MUX2)|(1<<MUX1));			break;
		case 7: ADCSRB &=  ~(1<<MUX5); 			ADMUX |=  ((1<<MUX2)|(1<<MUX1)|(1<<MUX0));												break;
		case 8: 	ADCSRB |=  1<<MUX5; 		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));												break;
	  	case 9:		ADCSRB |=  1<<MUX5; 		ADMUX &= ~((1<<MUX2)|(1<<MUX1));				ADMUX |=   1<<MUX0;						break;
	  	case 10: 	ADCSRB |=  1<<MUX5; 		ADMUX &= ~((1<<MUX2)|(1<<MUX0));				ADMUX |=   1<<MUX1;						break;
		case 11: 	ADCSRB |=  1<<MUX5; 		ADMUX &= ~(1<<MUX2);							ADMUX |= ((1<<MUX1)|(1<<MUX0));			break;	
		case 12: 	ADCSRB |=  1<<MUX5; 		ADMUX &= ~((1<<MUX1)|(1<<MUX0));				ADMUX |=   1<<MUX2;						break;
	  	case 13:	ADCSRB |=  1<<MUX5; 		ADMUX &= ~(1<<MUX1);							ADMUX |= ((1<<MUX2)|(1<<MUX0));			break;
	  	case 14: 	ADCSRB |=  1<<MUX5; 		ADMUX &= ~(1<<MUX0);							ADMUX |= ((1<<MUX2)|(1<<MUX1));			break;
		case 15: 	ADCSRB |=  1<<MUX5; 		ADMUX |=  ((1<<MUX2)|(1<<MUX1)|(1<<MUX0));												break;
	}

switch (ModeInput) 
	{ 
		case 0: ADMUX &= ~((1<<MUX4)|(1<<MUX3));						break;
	  	case 1:	ADMUX &=  ~(1<<MUX4);				ADMUX |= 1<<MUX3; 	break;
	  	case 2: ADMUX &=  ~(1<<MUX3);				ADMUX |= 1<<MUX4; 	break;
		case 3: ADMUX |=  ((1<<MUX4)|(1<<MUX3));						break;	
	}
 
switch (Prescaler) 
	{ 
		case 0: ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));										break;
	  	case 1:	ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1));				ADCSRA |=   1<<ADPS0;				break;
	  	case 2: ADCSRA &= ~((1<<ADPS2)|(1<<ADPS0));				ADCSRA |=   1<<ADPS1;				break;
		case 3: ADCSRA &= ~(1<<ADPS2);							ADCSRA |= ((1<<ADPS1)|(1<<ADPS0));	break;	
		case 4: ADCSRA &= ~((1<<ADPS1)|(1<<ADPS0));				ADCSRA |=   1<<ADPS2;				break;
	  	case 5:	ADCSRA &= ~(1<<ADPS1);							ADCSRA |= ((1<<ADPS2)|(1<<ADPS0));	break;
	  	case 6: ADCSRA &= ~(1<<ADPS0);							ADCSRA |= ((1<<ADPS2)|(1<<ADPS1));	break;
		case 7: ADCSRA |=  ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));										break;
	}

switch (AutoTrig) 
	{ 
		case 0: ADCSRA &= ~(1<<ADATE);		break;
		case 1: ADCSRA |=   1<<ADATE;		break;
	} 
	
	ADMUX |= 1<<ADLAR;			// старшие 8 бит в ADCH!

	ADCInterruptON();
	ADCON();
	ADCStartConversionON();
	
	

}



void ADCON(void)
{
	ADCSRA |= 1<<ADEN;				// ADC on
}

void ADCOFF(void)
{
	ADCSRA &= ~(1<<ADEN);			// ADC off
}

void ADCStartConversionON(void)
{
	ADCSRA |= 1<<ADSC;				// StartConversion on
}

void ADCStartConversionOFF(void)
{
	ADCSRA &= ~(1<<ADSC);			// StartConversion off
}

void ADCInterruptON(void)
{
	ADCSRA |= 1<<ADIE;
}

void ADCInterruptOFF(void)
{
	ADCSRA &= ~(1<<ADIE);
}

/*
SIGNAL(SIG_ADC)
{
	
}
*/

