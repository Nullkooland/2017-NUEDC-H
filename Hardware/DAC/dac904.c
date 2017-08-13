#include "dac904.h"
#include <arm_math.h>
TIM_HandleTypeDef htim13;
int16_t DAC904_dacValue[4096];
uint16_t offset;

void DAC904_Init(void)
{
	offset = 0;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	__HAL_RCC_TIM13_CLK_ENABLE();
	TIM_OC_InitTypeDef OC_Config;

	htim13.Instance = TIM13;
	htim13.Init.Prescaler = 83;
	htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim13.Init.Period = 1000;

	if (HAL_TIM_PWM_Init(&htim13) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	OC_Config.OCMode = TIM_OCMODE_PWM1;
	OC_Config.Pulse = 500;
	OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	OC_Config.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim13, &OC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

	for (uint16_t i = 0; i < 2048; i++)
	{
		DAC904_dacValue[i] = 0;
	}
	
	for (uint16_t i = 2048; i < 4096; i++)
	{
		DAC904_dacValue[i] = 16383;
	}

	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
}
/*
void DAC904_Start(void)
{
	for (uint16_t i = 0; i < 4096; i++)
	{
		GPIOF->BSRR = (uint32_t)GPIO_PIN_8 << 16U;
		GPIOC->ODR = DAC904_dacValue[i];
		GPIOF->BSRR = GPIO_PIN_8;
		Delay_us(10);
	}
}*/

void TIM8_UP_TIM13_IRQHandler(void)
{
	if (__HAL_TIM_GET_IT_SOURCE(&htim13, TIM_IT_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_IT(&htim13, TIM_IT_UPDATE);
		GPIOC->ODR = DAC904_dacValue[offset++];
		offset %= 4096;
	}
}