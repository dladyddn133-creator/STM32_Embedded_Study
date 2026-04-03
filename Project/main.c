#include "device_driver.h"
#include <stdio.h>

// timer.c 의 TIM4 인터럽트에서 1ms(또는 10ms)마다 1씩 증가시키는 전역 틱 변수
extern volatile unsigned int System_Tick; 

// 전체 시스템 하드웨어 초기화
static void System_Hardware_Init(void) 
{
    Clock_Init();
    Uart2_PC_Init(115200);      // PC 모니터링용
    // Uart1_WiFi_Init(115200); // 웹 모니터링용 (추후 활성화)
    
    TIM4_Tick_Init(10);         // 10ms 단위 틱 발생기 시작
    
    ADC1_Init();                // 수위(PA2), 조도(PA3) 센서
    // SPI1_Init();             // RFID용 통신망
    // SPI2_Init();             // 도트 매트릭스용 통신망
    
    Servo_Init();               // 문, 호스 서보
    Pump_Init();                // 워터 펌프
    Step_Init();                // 블라인드 스텝모터
    Env_LED_Init();             // 환경 표시 LED
    
    setvbuf(stdout, NULL, _IONBF, 0); // printf 버퍼링 해제
    printf("\n=== 5-Module AI Robot System Booting ===\n");
}

void Main(void)
{
    System_Hardware_Init();

    // 각 모듈별 실행 주기를 관리하기 위한 로컬 타이머 변수
    unsigned int last_env_check = 0;
    unsigned int last_water_check = 0;
    
    // 센서 데이터 저장 변수
    int water_level = 0;
    int light_level = 0;

    for(;;)
    {
        // ---------------------------------------------------------
        // [모듈 1] 출입 제어 (가장 빠르게 반응해야 하므로 루프마다 체크)
        // ---------------------------------------------------------
        /* if (RFID_Check_Card()) 
        {
            printf("Authorized! Opening Door...\n");
            Servo_Door_Set(90);  // 문 열림
            // 일정 시간 후 닫는 로직은 별도 타이머 틱으로 관리
        }
        */

        // ---------------------------------------------------------
        // [모듈 3] 환경 제어 (예: 500ms 마다 한 번씩만 체크)
        // ---------------------------------------------------------
        if (System_Tick - last_env_check >= 50) // 10ms * 50 = 500ms
        {
            last_env_check = System_Tick;
            
            light_level = ADC1_Read(3); // PA3 조도 센서 읽기
            
            if (light_level < 0x200) // 어두워지면
            {
                Env_LED_On();
                // Step_Move(10, 1); // 블라인드 열기 (조금씩 Non-blocking으로 움직이도록 추후 고도화 필요)
            }
            else // 밝으면
            {
                Env_LED_Off();
            }
        }

        // ---------------------------------------------------------
        // [모듈 2 & 4] 급수 제어 및 정밀화 (예: 1초 마다 한 번씩 체크)
        // ---------------------------------------------------------
        if (System_Tick - last_water_check >= 100) // 10ms * 100 = 1000ms (1초)
        {
            last_water_check = System_Tick;
            
            water_level = ADC1_Read(2); // PA2 수위 센서 읽기
            
            if (water_level < 1000) // 물탱크에 물이 없으면/화분이 마르면
            {
                Pump_Set(1, 50);          // 펌프 50% 파워로 가동
                Servo_Hose_Set(45);       // 호스를 45도 방향으로 조절
                printf("Watering... Level: %d\n", water_level);
            }
            else // 물이 충분하면
            {
                Pump_Set(0, 0);           // 펌프 정지
                Servo_Hose_Set(0);        // 호스 원위치
            }
        }

        // ---------------------------------------------------------
        // [모듈 5] HMI 및 통신 (상태 업데이트)
        // ---------------------------------------------------------
        /*
        DotMatrix_Show_Icon(SYSTEM_NORMAL); // 현재 상태 디스플레이
        */
    }
}