// 통합 헤더
#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// Uart.c

extern void Uart2_Init(int baud);
extern void Uart2_Send_Byte(char data);
extern void Uart2_RX_Interrupt_Enable(int en);

// Led.c

extern void LED_Init(void);
extern void LED_On(void);
extern void LED_Off(void);

// Clock.c

extern void Clock_Init(void);

// Key.c

extern void Key_ISR_Enable(int en);

// Timer.c

extern void TIM4_Repeat_Interrupt_Enable(int en, int time);

// Adc.c

extern void ADC1_IN6_Init(void);
extern void ADC1_Start(void);
extern void ADC1_Stop(void);
extern int ADC1_Get_Status(void);
extern int ADC1_Get_Data(void);

// motor.c (추가!)
extern void Motor_Init(void);
extern void Motor_Set(int dir, int duty);