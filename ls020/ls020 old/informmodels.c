#include "informmodels.h"


	
void GRAF(void)
{
	POLE();

}

void POLE(void)
{	


	line_vertical(Y0, Y1, X0 , sky, 16);	// постройка рамки для графика	
	line_horizontal(X0, X1, Y0, sky, 16);
	line_horizontal(X0, X1, Y1, sky, 16);
	line_vertical(Y0, Y1, X1 , sky, 16);
	

//	put_dot(KoordinaY0 + HeightGraf ,KoordinaX0 + WidthGraf ,brown,16);	
}
