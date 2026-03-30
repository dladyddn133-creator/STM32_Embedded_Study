#include "device_driver.h"

void MOTOR_Init(void)
{
	/* 아래 코드 수정 금지 : Port-A Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 0); 

	// PA0을 출력으로 설정하고 초기 OFF
	Macro_Write_Block(GPIOA->MODER, 0x3, 0x1, 0);
    // PA1을 출력으로 설정하고 초기 OFF
	Macro_Write_Block(GPIOA->MODER, 0x3, 0x1, 2);

	Macro_Clear_Bit(GPIOA->OTYPER, 0);
	Macro_Clear_Bit(GPIOA->OTYPER, 1);

	Macro_Clear_Bit(GPIOA->ODR, 0);
	Macro_Clear_Bit(GPIOA->ODR, 1); 
}
void MOTOR_CW(void)
{
	// MOTOR CW
	Macro_Set_Bit(GPIOA->ODR, 0);
	Macro_Clear_Bit(GPIOA->ODR, 1);    

}
void MOTOR_CCW(void)
{
    // MOTOR CCW
	Macro_Clear_Bit(GPIOA->ODR, 0);    
    Macro_Set_Bit(GPIOA->ODR, 1);
}
void MOTOR_STOP(void)
{
    Macro_Clear_Bit(GPIOA->ODR, 0);
    Macro_Clear_Bit(GPIOA->ODR, 1);
}