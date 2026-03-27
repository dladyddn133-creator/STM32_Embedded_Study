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

void Main(void)
{
	Sys_Init(115200);
	printf("UART Driver Test\n");

	Uart1_Init(115200);

	for(;;)
	{
		volatile int i;

		Uart1_Printf("\n키보드로 받는 글자 10개를 echo-back\n");
		Uart1_Printf("\n키보드를 누르면 다시 PC로 글자를 전송한다 (10글자)\n");

		for(i=0; i<10; i++)
		{
			Uart1_Printf("\n>");
			Uart1_Printf("%c", Uart1_Get_Char());
		}

		Uart1_Printf("\n계속 LED Toggling하고 있으며 키보드의 x를 누르면 종료 함");

		for(;;)
		{
			char x = Uart1_Get_Pressed();
			if(x != 0x0) Uart1_Printf("%c\n", x);
			if(x == 'x') break;
			
			LED_On();
			for(i=0; i<0x100000; i++);
			LED_Off();
			for(i=0; i<0x100000; i++);
		}
	}
}
