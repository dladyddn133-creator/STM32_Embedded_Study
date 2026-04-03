#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

volatile int Key_Pressed = 0;
volatile int Key_Pressed_2 = 0;


volatile int Uart_Data_In;
volatile unsigned char Uart_Data;

void Main(void)
{
	Sys_Init(115200);
    Uart1_Init(115200);

	Key_ISR_Enable(1);


	printf("ADC Test\n");
	MOTOR_CW();
	



}
