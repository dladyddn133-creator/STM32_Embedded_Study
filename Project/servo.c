#include "device_driver.h"

void Servo_Init(void)
{
    // 1. GPIOA 및 TIM2 클럭 활성화
    Macro_Set_Bit(RCC->AHB1ENR, 0); 
    Macro_Set_Bit(RCC->APB1ENR, 0); 

    // 2. PA0, PA1을 Alternate Function(AF) 모드(0x2)로 설정
    Macro_Write_Block(GPIOA->MODER, 0xF, 0xA, 0); // 0b1010
    
    // 3. PA0, PA1 모두 AF1(TIM2)로 맵핑 (0x11)
    Macro_Write_Block(GPIOA->AFR[0], 0xFF, 0x11, 0); 

    // 4. [96MHz 전용 세팅] 50Hz (20ms 주기) 생성
    // 96MHz / 96 = 1MHz (1us 틱)
    TIM2->PSC = 96 - 1;     
    // 1us * 20000 = 20,000us (20ms)
    TIM2->ARR = 20000 - 1;  

    // 5. CH1(PA0), CH2(PA1) PWM 모드 1 설정 및 Preload 활성화
    Macro_Write_Block(TIM2->CCMR1, 0xFFFF, 0x6868, 0); 
    
    // 6. [버그 수정] 마스크를 0xFF로 늘려 CH1, CH2 동시 출력 활성화
    Macro_Write_Block(TIM2->CCER, 0xFF, 0x11, 0);
    
    // 7. 카운터 시작
    Macro_Set_Bit(TIM2->CR1, 0);
}

// 출입문 각도 제어 (PA0 - TIM2_CH1)
void Servo_Door_Set(int angle)
{
    if(angle < 0) angle = 0;
    if(angle > 180) angle = 180;
    // 0~180도를 1000us(1ms) ~ 2000us(2ms) 펄스로 변환
    TIM2->CCR1 = 1000 + ((angle * 1000) / 180); 
}

// 호스 방향 제어 (PA1 - TIM2_CH2)
void Servo_Hose_Set(int angle)
{
    if(angle < 0) angle = 0;
    if(angle > 180) angle = 180;
    TIM2->CCR2 = 1000 + ((angle * 1000) / 180); 
}