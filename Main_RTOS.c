/*---------------------------------*/
/* ������ ������ � ���� - FreeRTOS */
/*---------------------------------*/

/* ����� FreeRTOS.h � task.h �����������
������ ���� �������� � ������ ������ FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"


/*
//---------------------------------//
//            ������ #1            //
//---------------------------------//
void vTask11( void *pvParameters )
{
// ������ ��������� �� ���� ������������ ����� //
 for(;;)
 {
// ������������� ��������� ���� PC0 //
  PORTE ^= (1 << PE4);
// �������� �� ��������� ������ �1 //
  vTaskDelay(250); // T1 = 200 ��������� �����
 }
// ���������� ������ ���� ��������� �����
�� ������������ ����� //
 vTaskDelete(NULL);
}

//---------------------------------//
//            ������ #2            //
//---------------------------------//
void vTask12( void *pvParameters )
{
 for(;;)
 {
// ������������� ��������� ���� PC1 //
  PORTE ^= (1 << PE3);
// �������� �� ��������� ������ �2 //
  vTaskDelay(500); // �2 = 150 ��������� �����
 }
 vTaskDelete(NULL);
}
*/


/*---------------------------------*/
/*            ������ #3            */
/*---------------------------------*/
void vTask15( void *pvParameters )
{
 for(;;)
 {
/* ������������� ��������� ���� PC1 */
  PORTC ^= (1 << PC2);
/* �������� �� ��������� ������ �2 */
  vTaskDelay(1000); // �2 = 150 ��������� �����
 }
 vTaskDelete(NULL);
}


/*---------------------------------*/
/*            ������ #4            */
/*---------------------------------*/
void vTask16( void *pvParameters )
{
 for(;;)
 {
/* ������������� ��������� ���� PC1 */
  PORTC ^= (1 << PC3);
/* �������� �� ��������� ������ �2 */
  vTaskDelay(2000); // �2 = 150 ��������� �����
 }
 vTaskDelete(NULL);
}





//---------------------------------//
//           Entry Point           //
//---------------------------------//
int main3333( void )
{
//���� 0,1 ����� C ����� �������� ��� ������ //
 DDRE |= ((1<<DDE3) | (1<<DDE4));

/*
// ��������� ������ #1. ��� ������ ������� xTaskCreate
��������� ������ ��� �� ������������, ���������������� ����������.
� ���� ������� �� ������������ �������� �� ���������� �������� ������ //
 xTaskCreate( vTask1, // ��������� �� ����������� ������� //
             (signed char *) "Task1", // ��� ������, ����������� ��� ������� FreeRTOS //
             configMINIMAL_STACK_SIZE,  // ������ ����� ������ //
             NULL,  // ������������ �������� //
             1,  // ��������� ������ //
             NULL );  // ���������� ������ //


// ��������� ������ #2 //
 xTaskCreate( vTask2, // ��������� �� ����������� ������� //
             (signed char * ) "Task2", // ��� ������, ����������� ��� ������� FreeRTOS //
             configMINIMAL_STACK_SIZE, // ������ ����� ������ //
             NULL, // ������������ �������� //
             1, // ��������� ������ //
             NULL ); // ���������� ������ //

*/

/* ��������� �����������. � ����� ����� ������ �������� ��� ��������� ����� ������.
����������� ������ ��������� ������ ������ � ����� ������� �����������.
���� ������� ��������� ����� � ������������ �����������, �� ��� ����� �������� ���������.
��� ���������� ������ FreeRTOS, ����������� ������ �������� �����.*/

 vTaskStartScheduler();
 return 0;
}
/*---------------------------------*/
/*           End of File           */
/*---------------------------------*/