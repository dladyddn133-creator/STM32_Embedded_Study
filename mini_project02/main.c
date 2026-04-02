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

volatile int Uart_Data_In;
volatile unsigned char Uart_Data;

void Main(void)
{
	Sys_Init(115200);
    MOTOR_Init();
    Uart1_Init(115200);

	Key_ISR_Enable(1);
	Key_PB9_ISR_Enable(1);


	printf("ADC Test\n");
	MOTOR_CW();
	
	// volatile int i;

	ADC1_IN6_Init();

    int current_speed = 9;

	for(;;)
	{
		if(Key_Pressed)
		{
			ADC1_Start();
			while(!ADC1_Get_Status());
			int key = ((int)ADC1_Get_Data()-1000)/100;

			if(key >= 0 && key <= 9)
			{
				current_speed = key; // 문자를 숫자로 변환
				MOTOR_Speed_Set(current_speed);
				Uart1_Printf("\n[Speed] Set to %d", current_speed);
			}
		}
		// printf("0x%.4X\n", ADC1_Get_Data());
		// for(i=0; i<0x400000; i++);
	}
}
