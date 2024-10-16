#include <stdio.h>
#include <util/delay.h>
#include "ds18x20.h"

extern char dataFAT [];

char DS18x20_StartMeasure(void)
{
	//Reset, skip ROM and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_WriteByte(OW_CMD_SKIPROM);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

char DS18x20_StartMeasureAddressed(unsigned char* rom)
{
	//Reset and start temperature conversion
	if (!OW_Reset()) return 0;
	OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

#ifdef DS18X20_CHECK_CRC
#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0

unsigned char crc8(unsigned char *data_in, unsigned int number_of_bytes_to_read )
{
	unsigned char	crc;
	unsigned int	loop_count;
	unsigned char	bit_counter;
	unsigned char	data;
	unsigned char	feedback_bit;

	crc = CRC8INIT;
	
	for (loop_count = 0; loop_count != number_of_bytes_to_read; loop_count++)
	{ 
		data = data_in[loop_count];

		bit_counter = 8;
		do { 
			feedback_bit = (crc ^ data) & 0x01;
			if (feedback_bit==0x01) crc = crc ^ CRC8POLY;

			crc = (crc >> 1) & 0x7F;
			if (feedback_bit==0x01) crc = crc | 0x80;

			data = data >> 1;
			bit_counter--;
		}
		while (bit_counter > 0);
	}
	return crc;
}
#endif 

char DS18x20_ReadData(unsigned char *rom, unsigned char *buffer)
{
	//Reset, skip ROM and send command to read Scratchpad
	if (!OW_Reset()) return 0;
	if (rom[0]==0)	OW_WriteByte(OW_CMD_SKIPROM);
	else OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_RSCRATCHPAD);
	
#ifdef DS18X20_CHECK_CRC
	unsigned char	buff[9] = {1,2,3,4,5,6,7,8,9};
	for (unsigned char i=0; i<9; i++) buff[i] = OW_ReadByte();
	buffer[0] = buff[0]; buffer[1] = buff[1];
	if (crc8(buff, 9)) return 0;	// ���� ����������� ����� �� �������, ���������� ������
#else 
	//Read Scratchpad (only 2 first bytes)
	buffer[0] = OW_ReadByte(); // Read TL
	buffer[1] = OW_ReadByte(); // Read TH	
#endif

	return 1;
}

void DS18x20_ConvertToThemperature(unsigned char* data, unsigned char* themp)
{
	//Store temperature integer digits and decimal digits
	themp[1] = data[0]>>4;
	themp[1] |= (data[1]&0x07)<<4;
	//Store decimal digits
	themp[2] = data[0]&0xf;
	themp[2] *= 5;	
	if (data[1]>0xFB){
		themp[1] = 127-themp[1];
		themp[0] = '-';
	} 
	else if((data[0]==0x00)&&(data[1]==0x00)) themp[0] = ' '; else themp[0] = '+';	
}





void print_address(unsigned char* address) 
{
	printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

unsigned char	nDevices;															// ���������� ��������
unsigned char	owDevicesIDs[MAXDEVICES][8];										// �� ID

unsigned char search_ow_devices(void) 												// ����� ���� ��������� �� ����
{ 
	unsigned char	i;
   	unsigned char	id[OW_ROMCODE_SIZE];
   	unsigned char	diff, sensors_count;

	sensors_count = 0;

	for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
    { 
		OW_FindROM( &diff, &id[0] );

      	if( diff == OW_PRESENCE_ERR ) break;

      	if( diff == OW_DATA_ERR )	break;

      	for (i=0;i<OW_ROMCODE_SIZE;i++)
         	owDevicesIDs[sensors_count][i] = id[i];
		
		sensors_count++;
    }
	return sensors_count;

}




void DallasTemp (void)
{

	nDevices = search_ow_devices(); 									// ���� ��� ����������

//	printf("\r---------- Found %d devices ----------", nDevices);	

		for (unsigned char i=0; i<nDevices; i++) 						// ������ ��������� ���������� � ����������� ������
		{
																		// ������ ���������� ����� �� ��� ��������� ����, ������� ���������� � ������ ����� �������
		switch (owDevicesIDs[i][0])
		{
			case OW_DS18B20_FAMILY_CODE: 
				{ 														// ���� ������ ����������� DS18B20
//				printf("\r"); print_address(owDevicesIDs[i]); 			// �������� ���� �������� ������, ����� - �����
//				printf(" - Thermometer DS18B20"); 						// �������� ��� ���������� 
				DS18x20_StartMeasureAddressed(owDevicesIDs[i]); 		// ��������� ���������							
				_delay_ms(1000);										// ���� ������� 750 ��, ���� �������������� �����������
				unsigned char	data[2]; 								// ���������� ��� �������� �������� � �������� ����� ������
				DS18x20_ReadData(owDevicesIDs[i], data); 				// ��������� ������
				unsigned char	themperature[3]; 						// � ���� ������ ����� �������� �����������
				DS18x20_ConvertToThemperature(data, themperature); 		// ��������������� ����������� � ���������������� ���
//				printf(": %d.%d C", themperature[1],themperature[2]);
				
				dataFAT [26] = 48 + themperature[1]/10;
				dataFAT [27] = 48 + themperature[1]%10;

				dataFAT [29] = 48 + themperature[2]*10/100;
				dataFAT [30] = 48 + (themperature[2]*10%100)/10;
				dataFAT [31] = 48 + (themperature[2]*10%100)%10;

				} 
				break;


			case OW_DS18S20_FAMILY_CODE: 
				{ 														// ���� ������ ����������� DS18B20
//				printf("\r"); print_address(owDevicesIDs[i]); 			// �������� ���� �������� ������, ����� - �����
//				printf(" - Thermometer DS18S20"); 						// �������� ��� ���������� 
				} 
				break;


			case OW_DS1990_FAMILY_CODE: 
				{ 														// ���� ������ ����������� ���� DS1990
//				printf("\r"); print_address(owDevicesIDs[i]); 			// �������� ���� �������� ������, ����� - �����
//				printf(" - Serial button DS1990"); 						// �������� ��� ���������� 
				} 
				break;


			case OW_DS2430_FAMILY_CODE: 
				{ 														// ���� ������� EEPROM
//				printf("\r"); print_address(owDevicesIDs[i]); 			// �������� ���� �������� ������, ����� - �����
//				printf(" - EEPROM DS2430"); 							// �������� ��� ���������� 
				} 
				break;


			case OW_DS2413_FAMILY_CODE: 
				{ 														// ���� ������ ����
//				printf("\r"); print_address(owDevicesIDs[i]); 			// �������� ���� �������� ������, ����� - �����
//				printf(" - Switch 2413"); 								// �������� ��� ���������� 
				} 
				break;


		}

	}

}
