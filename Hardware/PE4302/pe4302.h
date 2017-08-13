#pragma once
#include <stm32f4xx_hal.h>

//#define REMODE_MODE

#ifndef REMODE_MODE
#define LATCH_HIGH	GPIOF->BSRR = GPIO_PIN_8
#define LATCH_LOW	GPIOF->BSRR = (uint32_t)GPIO_PIN_8 << 16U

void PE4302_Init(void);
#endif

void PE4302_SetLoss(uint8_t channel, uint8_t twoTimes_dB);