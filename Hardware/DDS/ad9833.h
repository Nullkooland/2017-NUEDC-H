#pragma once
#include<stm32f4xx_hal.h>

#define FSYNC1		GPIOA->BSRR = GPIO_PIN_2
#define FSYNC0		GPIOA->BSRR = (uint32_t)GPIO_PIN_2 << 16U
#define SCLK1		GPIOA->BSRR = GPIO_PIN_4
#define SCLK0		GPIOA->BSRR = (uint32_t)GPIO_PIN_4 << 16U
#define SDATA1		GPIOA->BSRR = GPIO_PIN_6
#define SDATA0		GPIOA->BSRR = (uint32_t)GPIO_PIN_6 << 16U

static void WriteReg(uint16_t);
extern void Delay_us(uint32_t);
extern void Delay_ms(uint32_t);

void AD9833_Init(void);
void AD9833_WriteFreq(uint32_t);
void AD9833_SwitchMode(void);