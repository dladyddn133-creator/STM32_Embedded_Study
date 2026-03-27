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

/* Key.c의 옵션을 변경하면서 동작을 비교 실험 진행 */

void Main(void)
{
	Sys_Init(115200);
	printf("\nChattering 제거 방식 Key_Get_Pressed() 동작 시험\n");

	for(;;)
	{
		volatile int i;

		for(;;)
		{
			int k;

			if((k = Key_Get_Pressed()) != 0)
			{
				printf("\nKey Pressed!\n");
				Key_Wait_Key_Released();
			}

			else
			{
				printf("#");
				for(i = 0; i < 0x80000; i++);
			}
		}
	}
}
