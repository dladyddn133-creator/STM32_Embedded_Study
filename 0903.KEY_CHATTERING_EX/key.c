#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
}

/* 0: 디버깅용 설정 */
/* 1: 정상 동작용 설정 */

#if 0
	#define N 		20000
#else
	#define N 		3000000
#endif

static int Key_Check_Input(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);	
}

int Key_Get_Pressed(void)
{
	unsigned int i, k;

	for(;;)
	{
		k = Key_Check_Input();

		for(i = 0; i < N; i++)
		{
			if(k != Key_Check_Input())
			{
				break;
			}
		}

		if(i == N) break;
	}

	return k;
}

void Key_Wait_Key_Pressed(void)
{
	while(!Macro_Check_Bit_Clear(GPIOC->IDR, 13));
}

void Key_Wait_Key_Released(void)
{
	while(!Macro_Check_Bit_Set(GPIOC->IDR, 13));
}
