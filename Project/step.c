#include "device_driver.h"

void Step_Init(void)
{
    // 1. GPIOB 클럭 ON
    Macro_Set_Bit(RCC->AHB1ENR, 1); 

    // 2. PB8, PB9, PB10, PB14 핀을 GPIO Output(0x1) 모드로 설정
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x1, 16); // PB8 (IN1)
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x1, 18); // PB9 (IN2)
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x1, 20); // PB10(IN3)
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x1, 28); // PB14(IN4)
}

// 모터 1스텝 구동 함수 (내부용)
static void Step_Drive(int seq_idx)
{
    // 28BYJ-48 모터의 4-Step (Full-step) 제어 패턴
    const int step_pattern[4][4] = {
        {1, 0, 0, 0}, // 1번 패턴: PB8만 High
        {0, 1, 0, 0}, // 2번 패턴: PB9만 High
        {0, 0, 1, 0}, // 3번 패턴: PB10만 High
        {0, 0, 0, 1}  // 4번 패턴: PB14만 High
    };

    // 현재 GPIOB의 출력 상태를 읽어옴
    unsigned int out_data = GPIOB->ODR;
    
    // 제어할 4개의 핀만 0으로 초기화 (다른 PB 포트 보호)
    out_data &= ~((1<<8) | (1<<9) | (1<<10) | (1<<14)); 

    // 패턴에 맞춰 해당 핀을 1(High)로 설정
    if(step_pattern[seq_idx][0]) out_data |= (1<<8);
    if(step_pattern[seq_idx][1]) out_data |= (1<<9);
    if(step_pattern[seq_idx][2]) out_data |= (1<<10);
    if(step_pattern[seq_idx][3]) out_data |= (1<<14);

    // 실제 레지스터에 적용
    GPIOB->ODR = out_data;

    // [중요] 모터가 기계적으로 반응할 시간을 주는 딜레이
    // 이 숫자가 너무 작으면 윙~ 소리만 나고 헛돌며, 너무 크면 답답하게 돕니다.
    for(volatile int i=0; i<15000; i++); 
}

// 스텝모터 지정 횟수만큼 회전 (steps: 스텝 수, dir: 방향 1/-1)
void Step_Move(int steps, int dir)
{
    int seq = 0; // 패턴 인덱스 (0~3)
    
    for(int i = 0; i < steps; i++)
    {
        Step_Drive(seq);
        seq += dir; // 방향에 따라 패턴 순서를 앞으로 갈지 뒤로 갈지 결정
        
        if(seq > 3) seq = 0;
        if(seq < 0) seq = 3;
    }
    
    // [실무 팁] 이동이 끝나면 전류를 차단하여 모터 발열 방지
    unsigned int out_data = GPIOB->ODR;
    out_data &= ~((1<<8) | (1<<9) | (1<<10) | (1<<14)); 
    GPIOB->ODR = out_data;
}