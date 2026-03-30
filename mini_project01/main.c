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
	Uart1_Init(115200);
	
	volatile int i;
	MOTOR_Init();

	for(;;)
	{
		char key = Uart1_Get_Pressed();
		
		if(key != 0)
        {
            switch(key)
            {
                case 's': // Start (정회전)
					MOTOR_STOP();
					for(i=0;i<1000;i++);
                    MOTOR_CW();
                    Uart1_Printf("\n[Motor] Forward (CW)");
                    break;
                case 'r': // Reverse (역회전)
					MOTOR_STOP();
					for(i=0;i<1000;i++);
                    MOTOR_CCW();
                    Uart1_Printf("\n[Motor] Backward (CCW)");
                    break;
                case 'f': // Finish (정지)
                    MOTOR_STOP();
                    Uart1_Printf("\n[Motor] Stop");
                    break;
                default:
                    // 정의되지 않은 키는 무시
                    break;
            }
        }
	}
}