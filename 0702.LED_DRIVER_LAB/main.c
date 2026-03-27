#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
    SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
}

void Main(void)
{
    Sys_Init(115200);
    LED_Init();

    printf("\n--- LED Control System ---\n");
    printf("Press '0' to Turn ON, '1' to Turn OFF\n");

    for(;;)
    {
        // 사용자가 키를 누를 때까지 여기서 멈춰서 기다립니다.
        char key = Uart2_Get_Char();

        if(key == '0') 
        {
            LED_On();
            printf("Command Received: %c -> LED [ON]\n", key);
        }
        else if(key == '1')
        {
            LED_Off();
            printf("Command Received: %c -> LED [OFF]\n", key);
        }
        else 
        {
            // 다른 키를 눌렀을 때 안내 메시지
            printf("Invalid Key [%c]! Use '0' or '1'.\n", key);
        }
    }
}