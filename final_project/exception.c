#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

extern volatile int Key_Pressed;
extern volatile int Key_Pressed_2;


void EXTI15_10_IRQHandler(void)
{
	Key_Pressed = !Key_Pressed;
    Key_Pressed_2 = 0;

	
	EXTI->PR = 0x1 << 13;
	NVIC_ClearPendingIRQ(40);
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & (1 << 9))
    {
        // 1. 타이머 시작 (예: 50ms 대기 설정)
        SysTick_Run(50); 
        
        // 2. 50ms가 지날 때까지 CPU가 여기서 기다림 (Polling)
        while(!SysTick_Check_Timeout()); 
        
        // 3. 50ms 후에 버튼 상태를 다시 확인
        if(Macro_Check_Bit_Clear(GPIOB->IDR, 9)) 
        {
            // 실제 버튼 눌림 처리
			Key_Pressed = 0;
            Key_Pressed_2 = !Key_Pressed_2;
        }
        
        SysTick_Stop(); // 타이머 종료
        EXTI->PR = (1 << 9); 
    }
}
// extern volatile int Uart_Data_In;
// extern volatile unsigned char Uart_Data;

// void USART2_IRQHandler(void)
// {
// 	Uart_Data = (unsigned char)USART2->DR;
// 	Uart_Data_In = 1;
// 	NVIC_ClearPendingIRQ(38);
// }

// extern volatile int TIM4_Expired;

// void TIM4_IRQHandler(void)
// {
// 	// TIM4 Pending Clear
// 	Macro_Clear_Bit(TIM4->SR,0); 
// 	// NVIC Pending Clear
// 	NVIC_ClearPendingIRQ((IRQn_Type)30);
	
// 	TIM4_Expired = 1;
// }