#include "device_driver.h"
#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_MS  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 |= (0x1<<4)|(0x1<<3); 
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR=0xFFFF;
	// UG 이벤트 발생
	TIM2->EGR = (0x1<<0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1,0); 

}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1,0);
	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	// 계산된 time 값을 리턴(단위는 usec)
	time = (0xffff-TIM2->CNT)*20;
	return time;

}

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 |= ((0x1<<4)|(0x1<<3)); 
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = TIM2_PLS_MS * time;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR,0);

	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM2->SR,0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR,0);
	// Wait timeout
	while(Macro_Check_Bit_Clear(TIM2->SR,0));

	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM4 CR1: ARPE=0, down counter, repeat mode
	// PSC(50KHz),  ARR(reload시 값) 설정
	// UG 이벤트 발생
	// Update Interrupt Pending Clear
	// TIM4 start

}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴

}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}
