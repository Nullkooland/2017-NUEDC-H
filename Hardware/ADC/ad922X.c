#include "ad922X.h"

__IO int16_t AD922X_adcValue[4096];

TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_external_adc;

void AD922X_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_All;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_IC_InitTypeDef sConfigIC;

	__HAL_RCC_TIM3_CLK_ENABLE();

	htim3.Instance = TIM3;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.Period = 0xFFFF;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Prescaler = 0;

	if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	sClockSourceConfig.ClockFilter = 0;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 1;
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	__HAL_TIM_ENABLE_DMA(&htim3, TIM_DMA_UPDATE);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);

	hdma_external_adc.Instance = DMA1_Stream2;
	hdma_external_adc.Init.Channel = DMA_CHANNEL_5;
	hdma_external_adc.Init.Mode = DMA_NORMAL;
	hdma_external_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_external_adc.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_external_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_external_adc.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_external_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_external_adc.Init.MemInc = DMA_MINC_ENABLE;
	hdma_external_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;
	hdma_external_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_external_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_external_adc.Init.Priority = DMA_PRIORITY_VERY_HIGH;

	if (HAL_DMA_Init(&hdma_external_adc) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);

	/*
	__HAL_TIM_ENABLE(&htim3);

	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_DISABLE);
	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_CC4 | TIM_FLAG_CC4OF);
	HAL_DMA_Start(&hdma_external_adc, (uint32_t)&(GPIOC->IDR), (uint32_t)AD922X_adcValue, 4096);
	TIM_CCxChannelCmd(htim3.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);
	 
	HAL_DMA_Start(&hDMA, (u32)Buf, (u32)&(GPIOC->ODR), 5000);

	HAL_DMA_PollForTransfer(&hdma_external_adc, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
	*/
}

void TIM3_IRQHandler(void)
{
	if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC4) != RESET)
	{
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC4);
		AD922X_adcValue[0] = GPIOC->IDR;
	}
}