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

void Main(void)
{
	Sys_Init(115200);
	printf("\nNested Interrupt Test\n");

	NVIC_SetPriorityGrouping(3); 	// Binary Point = 4
	NVIC_SetPriority(30, 1); 		// TIM4
	NVIC_SetPriority(40, 2); 		// EXTI15_10
	NVIC_SetPriority(38, 3); 		// USART2

	Key_ISR_Enable(1);
	Uart2_RX_Interrupt_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, 100);

	volatile int i;
		
	for(;;)
	{
		printf(".");
		for(i=0; i<0x100000; i++);
	}
}
