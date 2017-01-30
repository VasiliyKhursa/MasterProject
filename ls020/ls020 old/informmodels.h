
#ifndef _INFORMMODELS_H
#define _INFORMMODELS_H

#include <avr/io.h>

#include "ls020.h"


#define Pole_H 14
#define Pole_W 8
#define Pole_COL 0x00
#define Pole_COL_WTR 0xAA

#define HeightGraf	100							// высота графика
#define WidthGraf	140							// ширина графика

#define KoordinaX0  30							// координаты начальной точки графика
#define KoordinaY0  20

#define 	X0  	KoordinaX0 - 1				// координаты начальной и конечной точек рамки для графика
#define 	X1  	X0 + WidthGraf + 2
#define 	Y0  	KoordinaY0 - 1
#define 	Y1  	Y0 + HeightGraf + 2



void GRAF(void);
void POLE(void);


#endif
