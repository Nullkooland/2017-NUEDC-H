#include "AD7606.h"

extern TIM_HandleTypeDef htim2;
__IO int16_t AD7606_adcValue[8];

void AD7606_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 3);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	AD7606_Reset();
	RD1;
	AD7606_SetSamplingFreq(1000U);
}
 
void AD7606_SetSamplingFreq(uint32_t samplingFreq) //Hz
{
	HAL_TIM_PWM_DeInit(&htim2);
	TIM2_PWM_Output_Init(100000U / samplingFreq);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

void AD7606_Reset(void)
{
	RESET0;
	RESET1;
	RESET1;
	Delay_us(100);
	RESET0;
}

void EXTI9_5_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_7) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);

		CS0;

		for (uint8_t i = 0; i < 8; i++)
		{
			RD0;
			asm("nop"); asm("nop"); asm("nop"); asm("nop");
			AD7606_adcValue[i] = GPIOC->IDR;
			RD1;
			asm("nop"); asm("nop");
		}

		CS1;
	}
}