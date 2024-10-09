#ifndef ADC_H
#define ADC_H

extern void InitADC(char Mode, char VolSel, char ModeInput, char Channel, char Prescaler, char AutoTrig);
extern void ADCON(void);
extern void ADCOFF(void);
extern void ADCStartConversionON(void);
extern void ADCStartConversionOFF(void);
extern void ADCInterruptON(void);
extern void ADCInterruptOFF(void);
//extern SIGNAL(SIG_ADC);

#endif
