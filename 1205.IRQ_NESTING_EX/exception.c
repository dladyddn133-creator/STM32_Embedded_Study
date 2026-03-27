#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

void EXTI15_10_IRQHandler(void)
{
	volatile int i;

	printf("\nEXTI15_10(40) IN\n");
	for(i=0; i<0x1000000; i++);
	printf("XTI15_10(40) OUT\n\n");
	
	EXTI->PR = 0x1 << 13;
	NVIC_ClearPendingIRQ(40);
}

void USART2_IRQHandler(void)
{
	volatile int i;

	printf("\nUSART2(38) IN: %c\n", (char)USART2->DR);
	for(i=0; i<0x1000000; i++);
	printf("USART2(38) OUT\n\n");

	NVIC_ClearPendingIRQ(37);
}

void TIM4_IRQHandler(void)
{
	static int d = 1;

	(d ^= 1) ? LED_On() : LED_Off();

	Macro_Clear_Bit(TIM4->SR, 0);
	NVIC_ClearPendingIRQ(30);
}
