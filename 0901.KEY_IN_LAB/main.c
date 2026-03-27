#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

/* Key 인식 */

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Test #1\n");

	/* 아래 코드 수정 금지 : Port-C Clock Enable */
	Macro_Set_Bit(RCC->AHB1ENR, 2); 

	// KEY(PC13)을 GPIO 입력으로 선언

	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 14);
	
	
	for(;;)
	{
		// KEY가 눌렸으면 LED(PA5) ON, 안 눌렸으면 OFF
		if (Macro_Check_Bit_Clear(GPIOC->IDR, 7))
		{
			LED_On();
		}
		else if(Macro_Check_Bit_Set(GPIOC->IDR,7))
		{
			LED_Off();
		}
	
	}
}

#endif

/* Key에 의한 LED Toggling */

#if 1

void Main(void)
{
	Sys_Init(115200);
	printf("KEY Input Toggling #1\n");

	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);

	int lock = 0;
	for(;;)
	{
		// KEY(PC13)이 눌릴때마다 LED(PA5)가 Toggling하도록 코드 작성
		if ((lock == 0)&& (Macro_Check_Bit_Clear(GPIOC->IDR,13)))
		{
			Macro_Invert_Bit(GPIOC->ODR, 5);
			lock=1;
		}
		else if ((lock == 1)&& (Macro_Check_Bit_set(GPIOC->IDR,13)))
		{
			lock=0;
		}
	}
}

#endif
