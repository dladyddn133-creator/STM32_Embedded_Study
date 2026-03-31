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
    MOTOR_Init();

    int current_speed = 9;
	volatile int i;
    for(;;)
    {
        char key = Uart1_Get_Pressed();

        if(key != 0)
        {
            if(key >= '1' && key <= '9') {
                current_speed = key - '0'; // 문자를 숫자로 변환
                MOTOR_Speed_Set(current_speed);
                Uart1_Printf("\n[Speed] Set to %d", current_speed);
            }
            else if(key == 's')
			{
				MOTOR_STOP();
				for(i=0;i<1000;i++);
				MOTOR_CW();
			}
            else if(key == 'r')
			{
				MOTOR_STOP();
				for(i=0;i<1000;i++);
				MOTOR_CCW();
			}
            else if(key == 'f') 
			MOTOR_STOP();
        }
    }
}