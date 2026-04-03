#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
}

int Key_Get_Pressed(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);	
}

void Key_Wait_Key_Pressed(void)
{
	while(!Macro_Check_Bit_Clear(GPIOC->IDR, 13));
}

void Key_Wait_Key_Released(void)
{
	while(!Macro_Check_Bit_Set(GPIOC->IDR, 13));
}

void Key_ISR_Enable(int en)
{
	if(en)
	{
		Macro_Set_Bit(RCC->AHB1ENR, 2); 
		Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);

		Macro_Set_Bit(RCC->APB2ENR, 14); 
		Macro_Write_Block(SYSCFG->EXTICR[3], 0xf, 0x2, 4);

		Macro_Set_Bit(EXTI->FTSR, 13);
		EXTI->PR = 0x1 << 13;
		
		NVIC_ClearPendingIRQ((IRQn_Type)40);
		Macro_Set_Bit(EXTI->IMR, 13);
		NVIC_EnableIRQ((IRQn_Type)40);
	}

	else
	{
		NVIC_DisableIRQ((IRQn_Type)40);
	}
}

void Key_PB9_ISR_Enable(int en)
{
    if(en)
    {
        // 1. GPIOB 클럭 활성화 (PB9이므로 AHB1ENR의 1번 비트)
        Macro_Set_Bit(RCC->AHB1ENR, 1); 

        // 2. PB9을 Input 모드(00)로 설정 (MODER 18번 비트부터 2비트)
        Macro_Write_Block(GPIOB->MODER, 0x3, 0x0, 18);

        // 3. Pull-up 설정 (PUPDR 18번 비트부터 2비트, 01 = Pull-up)
        // 스위치가 눌리면 GND로 연결되는 구조이므로 Pull-up이 필요합니다.
        Macro_Write_Block(GPIOB->PUPDR, 0x3, 0x1, 18);

        // 4. SYSCFG 클럭 활성화 (외부 인터럽트 매핑을 위해 필요)
        Macro_Set_Bit(RCC->APB2ENR, 14); 

        // 5. EXTI9를 Port-B로 연결 (SYSCFG->EXTICR[2])
        // EXTICR[2]는 EXTI8, 9, 10, 11을 담당합니다. 
        // EXTI9는 [7:4] 비트 위치에 있으며, Port-B이므로 값은 0x1입니다.
        Macro_Write_Block(SYSCFG->EXTICR[2], 0xf, 0x1, 4);

        // 6. Falling Edge 트리거 설정 (스위치를 누를 때 발생)
        Macro_Set_Bit(EXTI->FTSR, 9);
        
        // 7. 기존 펜딩 비트 클리어
        EXTI->PR = 0x1 << 9;

        // 8. NVIC 설정 (EXTI9_5 인터럽트 번호는 23번입니다)
        // F411xE 기준 EXTI9_5 인터럽트 번호는 23번입니다. (PC13은 40번이었음)
        NVIC_ClearPendingIRQ((IRQn_Type)23);
        Macro_Set_Bit(EXTI->IMR, 9);  // 인터럽트 마스크 해제
        NVIC_EnableIRQ((IRQn_Type)23);
    }
    else
    {
        NVIC_DisableIRQ((IRQn_Type)23);
    }
}
