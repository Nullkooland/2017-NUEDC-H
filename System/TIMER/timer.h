#pragma once
#include <stm32f4xx_hal.h>

void Delay_us(uint32_t);
void Delay_ms(uint16_t);
void TIM2_PWM_Output_Init(uint16_t);
void TIM6_DAC_Trigger_Init(void);
void TIM7_Base_Init(void);
void TIM14_PWM_Output_Init(uint16_t);
extern void _Error_Handler(char*, int);