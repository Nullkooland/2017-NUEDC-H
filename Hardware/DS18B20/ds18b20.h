#pragma once
#include <stm32f4xx_hal.h>

#define PRECISION		0x7F
#define SKIP_ROM		0xCC
#define START_CONVERT	0x44
#define	WRITE			0x4E
#define READ			0xBE

#define DS18B20_IO_SET GPIOB->BSRR = GPIO_PIN_12
#define DS18B20_IO_RESET GPIOB->BSRR = (uint32_t)GPIO_PIN_12 << 16U
#define DS18B20_IO_READBIT HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)

void DS18B20_Init(void);
void DS18B20_GetTemperature(void);

static void Mode_Input(void);
static void Mode_Output(void);
static void Write_Com(uint8_t);
static uint8_t Read_Byte(void);
static _Bool Reset(void);

extern void Delay_us(uint32_t);