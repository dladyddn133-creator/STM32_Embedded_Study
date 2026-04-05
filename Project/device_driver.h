#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

// System
extern volatile unsigned int System_Tick;
extern void Clock_Init(void);
extern void Uart2_Init(int baud);
extern void TIM4_Repeat_Interrupt_Enable(int en, int time);

// Sensors (PA2: 수위, PA3: 조도)
extern void ADC1_Init(void);
extern int ADC1_Read_Channel(int ch);

// Actuators
extern void Env_LED_Init(void);                // PB7
extern void Env_LED_On(void);
extern void Env_LED_Off(void);

extern void Step_Init(void);                   // PB8, 9, 10, 14
extern void Step_Move(int steps, int dir);

extern void Pump_Init(void);                   // PB1, PB2
extern void Pump_Set(int enable, int duty);

extern void Servo_Init(void);                  // PA0, PA1
extern void Servo_Door_Set(int angle);
extern void Servo_Hose_Set(int angle);