#include "device_driver.h"

void ADC1_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0);                 // GPIOA CLK ON
    
    // PA2, PA3를 Analog 모드(0x3)로 설정
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x3, 4);   // PA2
    Macro_Write_Block(GPIOA->MODER, 0x3, 0x3, 6);   // PA3

    Macro_Set_Bit(RCC->APB2ENR, 8);                 // ADC1 CLK ON
    Macro_Write_Block(ADC1->SMPR2, 0x3F, 0x3F, 6);  // CH2, CH3 샘플링 타임 설정
    Macro_Write_Block(ADC1->SQR1, 0xF, 0x0, 20);    // 1개씩 변환
    Macro_Write_Block(ADC->CCR, 0x3, 0x2, 16);      // ADC CLOCK
    Macro_Set_Bit(ADC1->CR2, 0);                    // ADC ON
}

int ADC1_Read_Channel(int ch)
{
    // 읽을 채널 선택
    Macro_Write_Block(ADC1->SQR3, 0x1F, ch, 0); 
    Macro_Set_Bit(ADC1->CR2, 30);                   // ADC 시작
    
    while(!Macro_Check_Bit_Set(ADC1->SR, 1));       // 변환 완료 대기
    
    int data = Macro_Extract_Area(ADC1->DR, 0xFFF, 0);
    Macro_Clear_Bit(ADC1->SR, 1);
    
    return data;
}