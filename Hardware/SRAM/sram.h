#pragma once
#include <stm32f4xx_hal.h>

#define FSMC_EXSRAM_BASE    0x68000000U	 //Bank1_SRAM3

void SRAM_Init(void);
void SRAM_Write(uint32_t, uint8_t*, uint32_t);
void SRAM_Read(uint32_t, uint8_t*, uint32_t);
static void HAL_FSMC_Init(void);
extern void _Error_Handler(char *, int);

