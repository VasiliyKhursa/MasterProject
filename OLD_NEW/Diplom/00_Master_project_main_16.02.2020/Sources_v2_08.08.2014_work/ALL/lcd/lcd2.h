#ifndef LCD2_H
#define LCD2_H


// Функции работы с LCD 
#define RS PH6 
#define EN PH4

void lcd_com(unsigned char p);
void lcd_data(unsigned char p);
void lcd_string(unsigned char command, char *string);
void lcd_num_to_str(unsigned int value, unsigned char nDigit);
void lcd_init(void);




#endif
