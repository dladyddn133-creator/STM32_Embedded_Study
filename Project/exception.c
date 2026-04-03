#include "device_driver.h"
#include <stdio.h>

// main.c에 있는 전역 변수들을 가져옴
extern volatile unsigned int System_Tick; // [수정됨] 전체 시스템 타이머
extern volatile char Uart_Data;
extern volatile int Uart_Data_In;
extern volatile int System_Mode;

// 만약 정의되지 않은 인터럽트가 발생하면 여기서 멈춤 (에러 디버깅용)
void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

// UART2 수신 인터럽트 (PA2, PA3 사용) - PC 콘솔 통신용
void USART2_IRQHandler(void) 
{
    Uart_Data = (char)USART2->DR; 
    Uart_Data_In = 1;
    
    // 인터럽트 Pending 비트 클리어 (CMSIS 기준 USART2_IRQn = 38)
    NVIC_ClearPendingIRQ(38); 
}

// PC13 유저 버튼 인터럽트 (모드 전환이나 비상 정지용으로 활용 가능)
void EXTI15_10_IRQHandler(void)
{
    // PC13 핀에서 발생한 인터럽트인지 확인
    if(Macro_Check_Bit_Set(EXTI->PR, 13))
    {
        // TODO: 버튼이 눌렸을 때 할 작업 (예: System_Mode++;)
        
        // EXTI Pending 비트 클리어 (1을 써서 지움)
        Macro_Set_Bit(EXTI->PR, 13); 
    }
    
    // NVIC Pending 비트 클리어 (CMSIS 기준 EXTI15_10_IRQn = 40)
    NVIC_ClearPendingIRQ(40);
}

// TIM4 인터럽트 (주기적인 심장 박동 생성기)
void TIM4_IRQHandler(void)
{
    // [핵심 변경] 플래그가 아니라 시간을 1씩 계속 증가시킵니다.
    // main.c에서 설정한 주기에 따라 (예: 10ms) System_Tick이 1씩 올라갑니다.
    System_Tick++; 
    
    // 타이머 인터럽트 플래그 클리어
    Macro_Clear_Bit(TIM4->SR, 0);
    
    // NVIC Pending 비트 클리어 (CMSIS 기준 TIM4_IRQn = 30)
    NVIC_ClearPendingIRQ(30);
}