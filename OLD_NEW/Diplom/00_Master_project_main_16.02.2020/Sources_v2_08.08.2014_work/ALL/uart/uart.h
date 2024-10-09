#ifndef UART_H
#define UART_H


extern void InitUART0(unsigned int BAUD,char CharacterSize, char StopBits, char Parity, char Mode);
extern void SendToUart0(unsigned char Data);
extern unsigned char ReceiveToUart0(void);

extern void InitUART1(unsigned int BAUD,char CharacterSize, char StopBits, char Parity, char Mode);
extern void SendToUart1(unsigned char Data);
extern unsigned char ReceiveToUart1(void);

extern void InitUART2(unsigned int BAUD,char CharacterSize, char StopBits, char Parity, char Mode);
extern void SendToUart2(unsigned char Data);
extern unsigned char ReceiveToUart2(void);

extern void InitUART3(unsigned int BAUD,char CharacterSize, char StopBits, char Parity, char Mode);
extern void SendToUart3(unsigned char Data);
extern unsigned char ReceiveToUart3(void);

#endif
