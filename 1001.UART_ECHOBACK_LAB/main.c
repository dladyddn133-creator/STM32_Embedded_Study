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

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("UART TX Test\n");

	Uart1_Init(115200);
	
	// A, B, C를 차례로 UART로 출력하는 코드 구현
	Uart1_Send_Byte('A');
	Uart1_Send_Byte('B');
	Uart1_Send_Byte('C');

}

#endif

#if 1

void Main(void)
{
	Sys_Init(115200);
	printf("\nUART Echo-Back Test\n");

	Uart1_Init(115200);

	for(;;)
	{
		unsigned char x;

		// PC에서 받은 글자를 다시 되돌려 보내는 코드 구현
		while(!Macro_Check_Bit_Set(USART1->SR, 5));
		x = USART1->DR;
		while(!Macro_Check_Bit_Set(USART1->SR, 7));
		USART1->DR = x;
	}
}

#endif
