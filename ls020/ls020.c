/* ***********************************************************************
**
**  Copyright (C) 2013  Vasilij Khursa
**
**  Siemens S65 
**
*********************************************************************** */

#include "ls020.h"

//unsigned int array[1] PROGMEM = {};

#if (Include6x8Font == 1)
	#include "font6x8.c"
#endif

#if (Include8x14Font == 1)
	#include "font8x14.c"
#endif

//------------------------------------------------------------------------------------------------------
// конcтанты цветов 16-бит
	
	uint16_t backcolor = black;    								// color of background
	uint16_t textcolor = white;


//------------------------------------------------------------------------------------------------------
// Команды инициализации.
unsigned char init_array_0[24] PROGMEM =
{	0xFD, 0xFD, 0xFD, 0xFD, 0xEF, 0x00, 0xEE, 0x04, 0x1B, 0x04, 0xFE, 0xFE,
 	0xFE, 0xFE, 0xEF, 0x90, 0x4A, 0x04, 0x7F, 0x3F, 0xEE, 0x04, 0x43, 0x06
};

unsigned char init_array_1[46] PROGMEM =
{	0xEF, 0x90, 0x09, 0x83, 0x08, 0x00, 0x0B, 0xAF,
 	0x0A, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 
 	0xEF, 0x00, 0xEE, 0x0C, 0xEF, 0x90, 0x00, 0x80, 
 	0xEF, 0xB0, 0x49, 0x02, 0xEF, 0x00, 0x7F, 0x01,
 	0xE1, 0x81, 0xE2, 0x02, 0xE2, 0x76, 0xE1, 0x83,
 	0x80, 0x01, 0xEF, 0x90, 0x00, 0x00
};




//------------------------------------------------------------------------------------------------------
// запись команды 8 бит
void vLs020WriteCmd8(char cmd8)									// Запись команды (8бит)
{

  	LS020_PORT_RS |= (1<<LS020_RS);								// Set RS
  	LS020_PORT_CS &= ~(1<<LS020_CS); 							// select LCD											
  	SPDR = cmd8;
  	while ( !(SPSR & (1 << SPIF)));
  	LS020_PORT_CS |= (1<<LS020_CS); 							// deselect LCD

}

//------------------------------------------------------------------------------------------------------
// запись команды 16 бит
void vLs020WriteCmd16(uint16_t  cmd16)								// Запись команды (16бит)
{
  	vLs020WriteCmd8((cmd16 >> 8) & 0xFF);
  	vLs020WriteCmd8(cmd16 & 0xFF);
}


//------------------------------------------------------------------------------------------------------
// запись данных 8 бит
void vLs020WriteData8(char data8)									// Запись данных
{

	LS020_PORT_RS &= ~(1<<LS020_RS); 							// Clear RS
	LS020_PORT_CS &= ~(1<<LS020_CS); 							// select LCD 
  	SPDR = data8;
  	while ( !(SPSR & (1 << SPIF)));
	LS020_PORT_CS |= (1<<LS020_CS); 							// deselect LCD 

}

//------------------------------------------------------------------------------------------------------
// запись данных 16 бит
void vLs020WriteData16(uint16_t data16)
{
 	vLs020WriteData8((data16>>8) & 0xFF);
	vLs020WriteData8(data16 & 0xFF);
}


//------------------------------------------------------------------------------------------------------
// Инициализация Дисплея
void vLs020Init(void)										// инициализация
{
  	LS020_DDR_RS  |= 1<<LS020_RS;
  	LS020_DDR_RST |= 1<<LS020_RST;
	LS020_DDR_CS  |= 1<<LS020_CS;

  	LS020_PORT_RS  |= 1<<LS020_RS;
  	LS020_PORT_RST |= 1<<LS020_RST;
	LS020_PORT_CS  |= 1<<LS020_CS;
			
	LS020_PORT_RST &= ~(1<<LS020_RST); 							// Pull RST pin down
//  PORTB &= ~(1<<RST); 										// Pull RST pin down
  	_delay_ms(10);

  	LS020_PORT_RST |= (1<<LS020_RST); 							// Release RST pin

  	for (unsigned char i=0; i<24; i++)
    	vLs020WriteCmd8(pgm_read_byte(init_array_0+i));
  	_delay_ms(7);
  	for (unsigned char i=0; i<46; i++)
    	vLs020WriteCmd8(pgm_read_byte(init_array_1+i));
}


//------------------------------------------------------------------------------------------------------
// Выбор 8-битного режима
// 0 - цветовое пространство RGB
// 1 - цветовое пространство RGB
void vLs020Mode8bit(char BGR) 									// BGR = 0 - disabled, BGR = 1 - enabled.
{
  	vLs020WriteCmd16(0xEF90);
	vLs020WriteCmd16(0xE800 + (BGR&0x01)*0x40);
}

// Выбор 16-битного режима
void vLs020Mode16bit(void)										// режим 16 бит
{
  	vLs020WriteCmd16(0xEF90);
	vLs020WriteCmd16(0xE80F);
}


//------------------------------------------------------------------------------------------------------
// Выбор расскладки дисплея
// Direction = 0 - книжная ориентация
// заполнение точек вертикальное, инкримент вертикальной координаты
// Direction = 1 - альбомная ориентация
// заполнение точек вертикальное, инкримент горизонтальной координаты
void vLs020SetDirection(char Direction)								
{
	vLs020WriteCmd16(0xEF90);
	vLs020WriteCmd16(0x0500 + Direction*0x0004);
}

//------------------------------------------------------------------------------------------------------
// Выбор частичного окна дисплея
// x1,y1 - координаты начальной точки
// x2,y2 - координаты конечной точки
void vLs020SetWindow(char x1, char y1, char x2,char y2, char Direction)
{	
	vLs020SetDirection(Direction);
  	vLs020WriteCmd16(0x0A00 + x1);
  	vLs020WriteCmd16(0x0B00 + x2);
  	vLs020WriteCmd16(0x0800 + y1);
  	vLs020WriteCmd16(0x0900 + y2);
}


//------------------------------------------------------------------------------------------------------
// Вывод точки заданного цвета
// bits_color = 8  - 8  bit mode
// bits_color = 16 - 16 bit mode
void vLs020PutDot(char x, char y, int color, char bits_color)						
{																				
  	vLs020SetDirection(0);
  	vLs020WriteCmd16(0x0800 + x);
  	vLs020WriteCmd16(0x0A00 + y);
	switch (bits_color)	
		{
		case 8:  vLs020Mode8bit(0); 	vLs020WriteData8(color & 0x00FF); 		break;
		case 16: vLs020Mode16bit(); 	vLs020WriteData16(color); 				break;
		}
	vLs020WriteCmd16(0x0800);
  	vLs020WriteCmd16(0x0A00);	

}

//------------------------------------------------------------------------------------------------------
// заливка полного экрана
void vLs020FillScreen(int color, char bits_color)					//	bits_color = 8   - 8  bit mode 
{																//	bits_color = 16  - 16  bit mode 
  	uint8_t color8;
	uint16_t i;

	vLs020SetWindow(0, 0, 175,131, 0);

	switch (bits_color)	
		{
		case 8: 
			vLs020Mode8bit(0); 	
			color8 = color & 0x00FF; 	
			for (i = 0; i<DISP_W*DISP_H; i++)
    			vLs020WriteData8(color8);  		
			break;

		case 16: 
			vLs020Mode16bit(); 	
			for (i = 0; i<DISP_W*DISP_H; i++)
    			vLs020WriteData16(color); 				
			break;
		}
}


//------------------------------------------------------------------------------------------------------
// Строит вертикальную линию по координатам у1 - начало линии, у2 - конец линии, 
// х - положение линиии по оси х, 
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020LineVertical(char y1, char y2, char x, int color, char bits_color)
{
	uint8_t color8,i;
	vLs020SetWindow(x, y1, x, y2, 1);

	switch (bits_color)	
		{
		case 8:  
				vLs020Mode8bit(0);
				color8 = color & 0x00FF;  	
				for( i = 0; i < (y2 - y1) + 1; i++)
					vLs020WriteData8(color8);
				break;

		case 16: 
				vLs020Mode16bit(); 	
				for( i = 0; i < (y2 - y1) + 1; i++)
					vLs020WriteData16(color);
				break;
		}

}

// Строит горизонтальную линию по координатам x1 - начало линии, x2 - конец линии, 
// y - положение линиии по оси y, 
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020LineHorizontal(char x1, char x2, char y, int color, char bits_color)
{
	uint8_t color8,i;
	vLs020SetWindow(x1, y, x2, y, 0);
	
	switch (bits_color)	
		{
		case 8:  
				vLs020Mode8bit(0); 
				color8 = color & 0x00FF; 	
				for( i=0; i<(x2-x1) + 1; i++)
					vLs020WriteData8(color8);
			break;
		
		case 16: 
				vLs020Mode16bit(); 	
				for( i=0; i<(x2-x1)+1; i++)
					vLs020WriteData16(color);
			break;
		}
	 
}

//------------------------------------------------------------------------------------------------------
// Строит линию через две произвольних точки
// не строит вертикальную линию
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020DrawLine(char x1, char y1, char x2, char y2, int color, char bits_color) 
{
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    
    int error = deltaX - deltaY;
    vLs020PutDot(x2, y2, color, bits_color);

    while(x1 != x2 || y1 != y2) 
		{
		vLs020PutDot(y1, x1, color, bits_color);
        const int error2 = error * 2;
        
        if(error2 > -deltaY) 
			{
            error -= deltaY;
            x1 += signX;
        	}
        if(error2 < deltaX) 
			{
            error += deltaX;
            y1 += signY;
        	}
    	}
}
//------------------------------------------------------------------------------------------------------
// Строит кольцо
// radius - радиус
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020DrawCircle(char x0, char y0, char radius, int color, char bits_color) 
{
        int x = 0;
        int y = radius;
        int delta = 2 - 2 * radius;
        int error = 0;
        while(y >= 0) 
		{
				vLs020PutDot(y0 + y, x0 + x, color, bits_color);
				vLs020PutDot(y0 - y, x0 + x, color, bits_color);
				vLs020PutDot(y0 + y, x0 - x, color, bits_color);
				vLs020PutDot(y0 - y, x0 - x, color, bits_color);
                
				error = 2 * (delta + y) - 1;
                if(delta < 0 && error <= 0) 
					{
                    ++x;
                    delta += 2 * x + 1;
                    continue;
                	}
                error = 2 * (delta - x) - 1;

                if(delta > 0 && error > 0) 
					{
                    --y;
                    delta += 1 - 2 * y;
                    continue;
                	}
                ++x;
                delta += 2 * (x - y);
                --y;
        }
}

//------------------------------------------------------------------------------------------------------
// Строит круг заполненный внутри алгоритмом Брезехэма
// radius - радиус
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020DrawCircleFull(char x0, char y0, char radius, int color, char bits_color) 
{
	char i;
	for (i = 0; i < radius + 1 ; i++)
		vLs020DrawCircle(x0, y0, i, color, bits_color);

}

//------------------------------------------------------------------------------------------------------
// Строит круг заполненный внутри линиями
// radius - радиус
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020DrawCircleFull2(char x0, char y0, char radius, int color, char bits_color)
{
        int x = 0;
        int y = radius;
        int delta = 2 - 2 * radius;
        int error = 0;
        while(y >= 0) 
			{
                vLs020LineVertical(y0 - y, y0 + y, x0 + x, color, bits_color);
				vLs020LineVertical(y0 - y, y0 + y, x0 - x, color, bits_color);
				error = 2 * (delta + y) - 1;
                if(delta < 0 && error <= 0) 
					{
                    ++x;
                    delta += 2 * x + 1;
                    continue;
                	}
                error = 2 * (delta - x) - 1;

                if(delta > 0 && error > 0) 
					{
                    --y;
                    delta += 1 - 2 * y;
                    continue;
                	}
                ++x;
                delta += 2 * (x - y);
                --y;
       		}
} 

//------------------------------------------------------------------------------------------------------
// Строит квадрат(прямоугольник) по двум точк незарисованный внутри
// color - цвет
// bits_color - кол-во бит цвета (8 или 16)
void vLs020DrawSquare(char x1, char y1, char x2, char y2, int color, char bits_color) 
{

	vLs020LineHorizontal(x1, x2, y1, color, bits_color);
	vLs020LineHorizontal(x1, x2, y2, color, bits_color);
	vLs020LineVertical(y1, y2, x1, color, bits_color);
	vLs020LineVertical(y1, y2, x2, color, bits_color);
}

//------------------------------------------------------------------------------------------------------



// 0 - normal mode 						- 	книжный режим
// 1 - х-axis symmetry (mirror image) 	- 	симметрия по оси Х (относитально точки x1y1)
// 2 - y-axis symmetry (mirror image) 	- 	симметрия по оси Y (относитально точки x1y1)
// 3 - 180 degree rotation 				- 	разворот на 180 градусов (относитально точки x1y1)
// 4 - mirror image, -90 degree   		- 	разворот на 90 градусов против часовой стрелке 
//											относительно книжной ориентации (относитально точки x1y1)
//											и симметрия о координате У
// 5 - +90 degree 						- 	разворот на 90 градусов по часовой стрелке 
//											относительно книжной ориентации (относитально точки x1y1)
//											альбоная ориентация
// 6 - -90 degree 						- 	разворот на 90 градусов против часовой стрелке 
//											относительно книжной ориентации (относитально точки x1y1)
//											альбоная ориентация перевернутая
// 7 - mirror image, +90 degree 		- 	разворот на 90 градусов по часовой стрелке 
//											относительно книжной ориентации (относитально точки x1y1)
//											и симметрия о координате Х

void vLs020Rotation(char x, char y, char CHARH, char CHARW, char mode)
{
	
	vLs020WriteCmd16(0xEF80);
	vLs020WriteCmd16(0x1800 + mode);

	switch(mode)
		{
		case  0:	
//				 	vLs020WriteCmd16(0x1800);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARH - 1));
					break;
	
		case  1:	
//				 	vLs020WriteCmd16(0x1801);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARH - 1));
					break;
		
		case  2:	
//				 	vLs020WriteCmd16(0x1802);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARH - 1));
					break;
		
		case  3:	
//				 	vLs020WriteCmd16(0x1803);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARH - 1));
					break;
		
		case  4:	
//				 	vLs020WriteCmd16(0x1804);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARW - 1));
					break;
		
		case  5:	
//					vLs020WriteCmd16(0x1805);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARW - 1));
					break;

		case  6:	
//					vLs020WriteCmd16(0x1806);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARW - 1));
					break;
		
		case  7:	
//					vLs020WriteCmd16(0x1807);
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARW - 1));
					break;
		}
}


void vLs020ZeroRotation(void)
{
	vLs020WriteCmd16(0xEF80);
	vLs020WriteCmd16(0x1800);
	vLs020WriteCmd16(ASX);
	vLs020WriteCmd16(AEX);
	vLs020WriteCmd16(ASY);
	vLs020WriteCmd16(AEY);
}


void vLs020Rotation_2(char x, char y, char CHARH, char CHARW, char mode)
{
	
	vLs020WriteCmd16(0xEF80);
	vLs020WriteCmd16(0x1800 + mode);

	switch(mode)
		{
		case  0:	
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARH - 1));
					break;
	
		case  1:	

					vLs020WriteCmd16(ASX + y);
//					vLs020WriteCmd16(AEX + y - (CHARW - 1));
					vLs020WriteCmd16(AEX + y + (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARH - 1));
					break;
		
		case  2:	

					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARH - 1));
					break;
		
		case  3:	

					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARW - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARH - 1));
					break;
		
		case  4:	

					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y + (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARW - 1));
					break;
		
		case  5:	
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x + (CHARW - 1));
					break;

		case  6:	
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARW - 1));
					break;
		
		case  7:	
					vLs020WriteCmd16(ASX + y);
					vLs020WriteCmd16(AEX + y - (CHARH - 1));
					vLs020WriteCmd16(ASY + x);
					vLs020WriteCmd16(AEY + x - (CHARW - 1));
					break;
		}
}

//------------------------------------------------------------------------------------------------------
// Вывод cимвола шрифта 8*14
// x - координата по горизонтали в режиме альбом
// у - координата по вертикали в режиме книга
// c - символ в кодировке ASCII
// rot = 1 - режим книга
// rot = 0 - режим альбом перевернутый

#if (Include8x14Font == 1)

void vLs020PutChar8x14(char x, char y, char symbol, int color, int bkcolor, char bits_color, char rot)
{
  	uint8_t h,ch,p,mask, color8,bkcolor8 ;

	vLs020Rotation(x, y, 14, 8, rot);
//	vLs020Rotation_2(x, y, 14, 8, rot);

//#define CHAR_H 14
//#define CHAR_W 8


	switch (bits_color)	
		{
		case 8:
			{	
			vLs020Mode8bit(0);
			color8 = color & 0x00FF;
			bkcolor8 = bkcolor & 0x00FF;

  			for (h = 0; h < CHAR_H; h++)									// every column of the character
  				{

       			ch = (pgm_read_byte(font8x14 + 14*(symbol - 32) + h));
				mask = 0x80;
        		
				for (p = 0; p < CHAR_W; p++)  								// write the pixels
        			{
          			if (ch & mask)
            			vLs020WriteData8(color8);                
          			else
            			vLs020WriteData8(bkcolor8);
					
					mask = mask/2;
					}        		
        		}
			}
			break;
	
		case 16:
			{
			vLs020Mode16bit();
  			for (h = 0; h < CHAR_H; h++)									// every column of the character
  				{

       			ch = (pgm_read_byte(font8x14 + 14*(symbol - 32) + h));
				mask = 0x80;
        		for (p = 0; p < CHAR_W; p++)  								// write the pixels
        			{
          			if (ch & mask)
            			vLs020WriteData16(color);                
          			else
            			vLs020WriteData16(bkcolor);
          			
					mask = mask/2;
					}
				}
			}
			break;
		} 

	vLs020Mode8bit(0);
	vLs020ZeroRotation();
}

#endif

//------------------------------------------------------------------------------------------------------
// Вывод cимвола шрифта 6*8
// x - координата по горизонтали в режиме альбом
// у - координата по вертикали в режиме книга
// symbol - символ в кодировке ASCII
// color   - цвет символа 
// bkcolor - цвет фона

#if (Include6x8Font == 1)

void vLs020PutChar6x8(char x, char y, char symbol, int color, int bkcolor, char bits_color, char rot)
{	
	uint8_t color8,bkcolor8 ;

	if (rot < 4) rot = rot + 4; 
		else rot = rot - 4;

	vLs020Rotation(x, y, 6, 8, rot);

//	vLs020Rotation(x, y, 14, 8, 0);
//	vLs020SetWindow(x, y, x + 7, y + 5, 1);

	int offset = 6*(symbol);

	switch (bits_color)	
		{
		case 8: 
			vLs020Mode8bit(0);
			color8 = color & 0x00FF;
			bkcolor8 = bkcolor & 0x00FF;
			for (char i = 0; i < 6 ; i++)
    			for (char j = 0; j<8; j++)
	  				if (((pgm_read_byte(font6x8 + offset + i) >> j) & 0x80) == 0x01)
	    					vLs020WriteData8(color8);
	  				else
	    					vLs020WriteData8(bkcolor8);		
			break;

		case 16: 
		 	vLs020Mode16bit();
			for (char i = 0; i < 6 ; i++)
    			for (char j = 0; j<8; j++)
	  				if (((pgm_read_byte(font6x8 + offset + i) >> j) & 0x01) == 0x01)
	    					vLs020WriteData16(color);
	  				else
	    					vLs020WriteData16(bkcolor);	
			break;
		
		}
  	
	vLs020Mode8bit(0);
	vLs020ZeroRotation();
  	
}

#endif

//------------------------------------------------------------------------------------------------------
// Вывод строки символов шрифта 6*8
// x - координата по горизонтали в режиме альбом
// у - координата по вертикали в режиме книга
// text - строка в кодировке ASCII
// color   - цвет символа 
// bkcolor - цвет фона
#if (Include6x8Font == 1)

void vLs020PutString6x8(char x, char y, char * text, int color, int bkcolor, char bits_color, char rot)
{
  	unsigned char i = 0; 
  	unsigned char y0 = 0, x0 = 0;
/*  	
	while (text[i]!=0)
  		{
    	vLs020PutChar6x8(x,y+y0,text[i], color,bkcolor, bits_color, rot);
    	i++;
    	y0+=6;
  		}
*/	
	switch (rot)
		{
		case 0:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x, y + y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=6;
  				}
			break;

		case 1:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x, y - y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=6;
  				}
			break;
			
		case 2:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x, y + y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=6;
  				}
			break;					

		case 3:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x, y - y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=6;
  				}
			break;

		case 4:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x + x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=6;
  				}
			break;

		case 5:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x + x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=6;
  				}
			break;
			
		case 6:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x - x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=6;
  				}
			break;					

		case 7:
			while (text[i] != 0)
  				{
				vLs020PutChar6x8(x - x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=6;
  				}
			break;
		}
	vLs020ZeroRotation();

}

#endif

//------------------------------------------------------------------------------------------------------
// Вывод строки символов шрифта 8*14
// x - координата по горизонтали в режиме альбом
// у - координата по вертикали в режиме книга
// text - строка в кодировке ASCII
// color   - цвет символа 
// bkcolor - цвет фона

#if (Include8x14Font == 1)

void vLs020PutString8x14(char x, char y, char * text, int color, int bkcolor, char bits_color, char rot)
{
  	unsigned char i = 0; 
  	unsigned char y0 = 0, x0 = 0;
  	
/*
	while (text[i] != 0)
  		{
		vLs020PutChar8x14(x, y + y0, text[i], color, bkcolor, bits_color, rot);
    	i++;
    	y0+=8;
  		}
*/
	switch (rot)
		{
		case 0:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x, y + y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=8;
  				}
			break;

		case 1:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x, y - y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=8;
  				}
			break;
			
		case 2:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x, y + y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=8;
  				}
			break;					

		case 3:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x, y - y0, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			y0+=8;
  				}
			break;

		case 4:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x + x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=8;
  				}
			break;

		case 5:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x + x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=8;
  				}
			break;
			
		case 6:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x - x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=8;
  				}
			break;					

		case 7:
			while (text[i] != 0)
  				{
				vLs020PutChar8x14(x - x0, y, text[i], color, bkcolor, bits_color, rot);
    			i++;
    			x0+=8;
  				}
			break;
		}
	vLs020ZeroRotation();
}

#endif

//------------------------------------------------------------------------------------------------------
// Вывод прямоугольника заданных размеров и цвета
void vLs020Rectangle(char x1, char y1, char x2, char y2, int color, char bits_color)
{	
	vLs020SetWindow(x1,y1,x2,y2,1);
	uint8_t color8 ;
	switch (bits_color)	
		{
		case 8: 
			color8 = color & 0x00FF;
			vLs020Mode8bit(0);  
			for (char y = y1; y <= y2; y++)
    			for (char x = x1; x <= x2; x++)
    				vLs020WriteData8(color8);		
			break;

		case 16: 
		 	vLs020Mode16bit();
			for (char y = y1; y <= y2; y++)
    			for (char x = x1; x <= x2; x++)
    				vLs020WriteData16(color);
			break;
		
		}
  	vLs020Mode8bit(0);
}




void vLs020Picture()
{
 	vLs020Mode16bit();
// 	unsigned short i;
 	vLs020WriteCmd16(0xEF90);
 	vLs020WriteCmd16(0x0500);
 	vLs020WriteCmd16(0x0800); //x1
 	vLs020WriteCmd16(0x0A00); //y1
 	vLs020WriteCmd16(0x0983); //x2
 	vLs020WriteCmd16(0x0BAF); //y2
 /*
 	for (i=0; i<(...); i++)
		vLs020WriteData16(pgm_read_word(array+i));
 */
 	vLs020Mode8bit(0);
}

/*
void decode_file(void)
{
	
	BYTE status = disk_initialize(0); 
//	sprintf((char*)buf,"initialize return: %X",status); 
//	xmit(buf);

	f_err_code = f_mount(0, &FATFS_Obj); 
//	sprintf((char*)buf,"f_mount return: %X",f_err_code); 
//	xmit(buf);

	unsigned int counter = 0;
	unsigned int counter_2 = 0;
	char i;
	unsigned char var_buff = 50;
	DWORD var_buff_pointer = 0;

//	DWORD lseek_pointer = 185241 - 250 ;
	DWORD lseek_pointer = 0 ;

	char var[var_buff];			 	
	UINT ByteRead = 0;
	
	DWORD lenth_file = 185241;
//	DWORD lenth_file = 100;

	char buff_2 [] = "  ";
	char buff_3 [] = "   ";
	char buff_4 [] = "     ";


	xmit("Decode array of picture"); 


	f_err_code = f_open(&fil_obj2, "picture.txt", FA_WRITE);
	sprintf((char*)buf,"f_open return: %X",f_err_code);
	xmit(buf);


	f_err_code = f_lseek(&fil_obj2,0);
	sprintf((char*)buf,"f_lseek return: %X, size of file: %d",f_err_code,(int)fil_obj2.fsize); 
	xmit(buf);


	f_err_code = f_open(&fil_obj, "1.txt", FA_READ); 
	sprintf((char*)buf,"f_open return: %X",f_err_code); 
	xmit(buf);



	do 
	{
	f_err_code = f_read(&fil_obj,var,var_buff,&ByteRead); 
//	sprintf((char*)buf,"var: %s",var);
//	xmit(buf);

	var_buff_pointer = var_buff;
//	sprintf((char*)buf,"var_buff: %d\n",var_buff);
//	xmit(buf);

	for(i = 0; i < var_buff; i++)
		{
		if (var[var_buff - 1 - i] != ',')
			var_buff_pointer --;
			else break;
		}
	
//	sprintf((char*)buf,"var_buff_pointer: %d\n",var_buff_pointer);
//	xmit(buf);


	for(i = 0; i < var_buff_pointer; i++ )
		{
		buff_2[0] = var [i];
		buff_2[1] = var [i+1];
		
//		sprintf((char*)buf,"buff_2: %s\n",buff_2);
//		xmit(buf);
		
		// find text "0x" in var ("0x" part of "0x0000") - find Color
		if (buff_2[0] == '0' && buff_2[1] == 'x')
			{
//			sprintf((char*)buf,"buff_2buff_2: %s\n",buff_2);
//			xmit(buf);
		
			if (var[i+1+6] == ',') 		n = 5;
			
			if (var[i+1+5] == ',') 		n = 4;

			if (var[i+1+4] == ',')		n = 3;

			if (var[i+1+3] == ',') 		n = 2;
	
			if (var[i+1+2] == ',') 		n = 1;

//			sprintf((char*)buf,"n=: %d\n",n);
//			xmit(buf);

			counter++;
			unsigned int a;
			unsigned char a_high = 0, a_low = 0;

			unsigned char* a_buff[2]={0,0};

			switch(n)
				{
				case 4 : 
						buff_4[0] = var [i+2]; buff_4[1] = var [i+3]; buff_4[2] = var [i+4]; buff_4[3] = var [i+5]; 
						unsigned int a4 = strtol(buff_4 ,NULL ,16);
						a = a4;
//						sprintf((char*)buf,"n4=: %s %d",buff_4,a4 );
//						xmit(buf);

						break;
				case 3 : 
						buff_3[0] = var [i+2]; buff_3[1] = var [i+3]; buff_3[2] = var [i+4];  
						unsigned int a3 = strtol(buff_3 ,NULL ,16);
						a = a3;
//						sprintf((char*)buf,"n3=: %s %d",buff_3,a3);
//						xmit(buf);

						break;
				case 2 : 
						buff_2[0] = var [i+2]; buff_2[1] = var [i+3];   
						unsigned int a2 = strtol(buff_2 ,NULL ,16);
						a = a2;
//						sprintf((char*)buf,"n2=: %s% %d",buff_2,a2);
//						xmit(buf);

						break;	
				}

//			a_high = (0x00FF&(a >>8));
//			sprintf((char*)buf,"a_high= %d",a_high );
//			xmit(buf);
			varr[0] = (0x00FF&(a >>8));

//			a_low = (0x00FF&(a));
//			sprintf((char*)buf,"a_low= %d",a_low );
//			xmit(buf);
			varr[1] = (0x00FF&(a));
			
			if (counter <= 23232)
				{
				counter_2 +=2;

				//write//

				f_err_code = f_write(&fil_obj2,varr,strlen(varr),&ByteWrite); 
//				sprintf((char*)buf,"f_write return: %X, byte wrote: %d",f_err_code,ByteWrite); 
//				xmit(buf);

				f_err_code = f_lseek(&fil_obj2,counter_2);
//				sprintf((char*)buf,"f_lseek return: %X, size of file: %d",f_err_code,(int)fil_obj2.fsize); 
//				xmit(buf);

				}


			}
		}
	
	lseek_pointer += var_buff_pointer;

	f_err_code = f_lseek(&fil_obj, lseek_pointer);	
//	sprintf((char*)buf,"f_lseek return: %X, byte pointer: %d",f_err_code, lseek_pointer); 
//	xmit(buf);

	}
	while(lseek_pointer < lenth_file);
	
	sprintf((char*)buf,"counter return= %d", counter ); 
	xmit(buf);

	sprintf((char*)buf,"counter_2 return= %d", counter_2 ); 
	xmit(buf);

	sprintf((char*)buf,"f_lseek return: %X, size of file: %d",f_err_code,(int)fil_obj2.fsize); 
	xmit(buf);

	f_close(&fil_obj);
	f_close(&fil_obj2);
}

*/






//----------------------------------------------------------------------------------------------------------------------------------------
// old_status, new_status - состояние Прогресс Бара старое и новое соотвестственно
// char width, char high - ширина и высота прогресс бара соответственно
// rot = 1 - режим книга
// rot = 0 - режим альбом перевернутый
// char start_pos_x, char start_pos_y - в альбомном(нормальном) режиме координаты начальной точки(нижняя левая точка прогресс бара)

/* тестовый запуск:
	for (char i = 0; i<=100; i++)
		{
		vLs020StatusBar(10, 10, 50, 12, black, green, black, 16, 0, i-1, i);
		_delay_ms(100);
		}
*/
// тестовый запуск:
//	vLs020StatusBar(10, 10, 102, 12, black, green, black, 16, 1, 0, 99);

void vLs020StatusBar(uint8_t start_pos_x,uint8_t start_pos_y,uint8_t width,uint8_t high, int color_border, int color_fon, int color_text, uint8_t bits_color, uint8_t rot, uint8_t old_status, uint8_t new_status)
{
	unsigned char Xstolb; 					// колличество столбиков внутри прогресс бара
	unsigned char Xpercent,Xpercent_ostat; 	// колличество столбиков на оддин процент прогресс бара

	char Data [] = "AAA";					// строка для вывода текста

	Xstolb = width - 2; 					// минус две стенки	
	Xpercent = Xstolb/100;
	Xpercent_ostat = Xstolb%100;
	
//	itoa(Xstolb,Data,10);					// преобразования числа в строку
//	vLs020PutString6x8(90, 10 , Data, 0xAAAA, 0x0000, 16, 0);
	

//	itoa(Xpercent,Data,10);						// преобразования числа в строку
//	vLs020PutString6x8(130, 10 , Data, 0xAAAA, 0x0000, 16, 0);

//	itoa(Xpercent_ostat,Data,10);						// преобразования числа в строку
//	vLs020PutString6x8(140, 10 , Data, 0xAAAA, 0x0000, 16, 0);
	

	uint8_t X_start =  start_pos_x + 1 + Xpercent*old_status;
	uint8_t X_stop  =  start_pos_x + width - 1 - Xpercent*(100 - new_status)- Xpercent_ostat*(100 - new_status)/100 ;	


//	itoa(X_start,Data,10);					// преобразования числа в строку
//	vLs020PutString6x8(100, 10 , Data, 0xAAAA, 0x0000, 16, 0);
//	itoa(X_stop,Data,10);					// преобразования числа в строку
//	vLs020PutString6x8(110, 10 , Data, 0xAAAA, 0x0000, 16, 0);

	if (X_stop > (start_pos_x + width - 1)) X_stop = start_pos_x + width - 1;

//	itoa(X_stop,Data,10);					// преобразования числа в строку
//	vLs020PutString6x8(120, 10 , Data, 0xAAAA, 0x0000, 16, 0);

	itoa(new_status,Data,10);
	
	if (rot == 0)
		{
		// рамочка прогресс бара - альбомный режим
		vLs020DrawSquare(start_pos_x, start_pos_y, start_pos_x + width, start_pos_y + high, color_border, bits_color);	
		
		for (uint8_t i = X_start; i <= X_stop; i++ )
			vLs020LineVertical(start_pos_y + 1, start_pos_y + high - 1, i, color_fon, bits_color);	

		// вывод текста 		
		if (high >=16)
			{
			
			#if (Include8x14Font == 1)

			vLs020PutString8x14((start_pos_x + width) / 2 - 8, start_pos_y + (high + 14) / 2, Data, color_text, color_fon, bits_color, 5);		
			vLs020PutChar8x14((start_pos_x + width) / 2 - 8 + 24, start_pos_y + (high + 14) / 2, '%', color_text, color_fon, bits_color, 5);
			
			#endif			
			}

			else if (high >=8)
				{
				#if (Include6x8Font == 1)

				vLs020PutString6x8((start_pos_x + width) / 2 - 6, start_pos_y + (high + 8) / 2, Data, color_text, color_fon, bits_color, 5);		
				vLs020PutChar6x8((start_pos_x + width) / 2 - 6 + 18, start_pos_y + (high + 8) / 2, '%', color_text, color_fon, bits_color, 5);

				#endif
				}
		}	

	if (rot == 1)
		{
		
		// рамочка прогресс бара - книжный режим		
		vLs020DrawSquare(start_pos_x, start_pos_y, start_pos_x + high, start_pos_y + width, color_border, bits_color);
		
		// прорисовка столбиков
		for (char i = X_start; i <= X_stop; i++ )
			vLs020LineHorizontal(start_pos_x + 1, start_pos_x + high - 1, i, color_fon, bits_color);
					
		// вывод текста 
		if (high >=16)
			{
			#if (Include8x14Font == 1)

			vLs020PutString8x14( start_pos_x + (high - 14) / 2,(start_pos_y + width) / 2 - 8, Data, color_text, color_fon, bits_color, 0);
			vLs020PutChar8x14( start_pos_y + (high - 14) / 2,(start_pos_x + width) / 2 - 8 + 24, '%', color_text, color_fon, bits_color, 0);
			
			#endif			
			}
			else if (high >=8)			
				{
				#if (Include6x8Font == 1)

				vLs020PutString6x8( start_pos_x + (high - 8) / 2,(start_pos_y + width) / 2 - 6, Data, color_text, color_fon, bits_color, 0);
				vLs020PutChar6x8( start_pos_y + (high - 8) / 2, (start_pos_x + width) / 2 - 6 + 18, '%', color_text, color_fon, bits_color, 0);
				
				#endif
				}
		}	


}











