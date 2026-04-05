// Pump_Init(void), Pump_Set(int enable, int duty) - 워터펌프(PB1, PB2) PWM 제어

#include "device_driver.h"

void Pump_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 1); 
    Macro_Set_Bit(RCC->APB1ENR, 1); // TIM3 CLK ON

    Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 2); // PB1: AF 모드 (PWM)
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x1, 4); // PB2: Out 모드 (DIR)
    Macro_Write_Block(GPIOB->AFR[0], 0xF, 0x2, 4); // PB1 -> AF2 (TIM3_CH4)

    TIM3->PSC = 960 - 1; 
    TIM3->ARR = 100 - 1; 
    Macro_Write_Block(TIM3->CCMR2, 0xFF00, 0x6800, 0); 
    Macro_Set_Bit(TIM3->CCER, 12); 
    Macro_Set_Bit(TIM3->CR1, 0);
}

void Pump_Set(int enable, int duty)
{
    if(enable)
    {
        Macro_Set_Bit(GPIOB->ODR, 2); // 정방향 회전
        TIM3->CCR4 = duty;            // 속도 (0~100)
    }
    else
    {
        Macro_Clear_Bit(GPIOB->ODR, 2); 
        TIM3->CCR4 = 0;
    }
}