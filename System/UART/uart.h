#pragma once
#include <stm32f4xx_hal.h>

#define UART1_RX_BUFFER_SIZE 2048

void UART1_Init(void);
extern void _Error_Handler(char *, int);