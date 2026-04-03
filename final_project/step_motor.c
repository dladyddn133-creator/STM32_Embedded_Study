#include "device_driver.h"

void STEP_MOTOR_Init(void)
{
    //TODO
    
    // Macro_Set_Bit(RCC->AHB1ENR, 0);   // Port A Clock
    // Macro_Set_Bit(RCC->APB1ENR, 0);   // TIM2 Clock

    // // PA0, PA1을 Alternate Function(10)으로 설정
    // Macro_Write_Block(GPIOA->MODER, 0xF, 0xA, 0);
    // // PA0, PA1에 AF1(TIM2) 연결
    // Macro_Write_Block(GPIOA->AFR[0], 0xFF, 0x11, 0);

    // // TIM2 설정 (5kHz)
    // TIM2->PSC = 19199;
	// // 요청한 주파수가 되도록 ARR 설정
	// TIM2->ARR = 1999;
    
    // // PWM Mode 1 설정 (CH1, CH2)
    // TIM2->CCMR1 = (6 << 4) | (6 << 12);
    // // 출력 활성화
    // TIM2->CCER = (1 << 0) | (1 << 4);
    
    // TIM2->CR1 = (1 << 0); // Timer Start
    
    // MOTOR_Speed_Set(9); // 초기 속도 9
    // MOTOR_STOP();
}
void MOTOR_Speed_Set(int step)
{
    if (step < 1) step = 1;
    if (step > 9) step = 9;
    
    // 9단계 분할 (ARR=200 기준, 단계당 약 22씩 증가)
    unsigned int duty = step * 199; 
    
    // 현재 방향에 맞춰 CCR 업데이트 (전역 변수로 방향 관리 권장)
    // 여기서는 간단히 두 채널 모두 업데이트 로직에 활용
    TIM2->CCR1 = duty;
    TIM2->CCR2 = duty;
}

// CW/CCW 시 한쪽은 PWM, 한쪽은 0으로 만들어야 회전함
void MOTOR_CW(void) {
    TIM2->CCER |= (1 << 0);  // CH1 ON
    TIM2->CCER &= ~(1 << 4); // CH2 OFF
}

void MOTOR_CCW(void) {
    TIM2->CCER &= ~(1 << 0); // CH1 OFF
    TIM2->CCER |= (1 << 4);  // CH2 ON
}

void MOTOR_STOP(void) {
    TIM2->CCER &= ~((1 << 0) | (1 << 4)); // 둘 다 OFF
}