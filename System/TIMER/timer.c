#include "timer.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim14;

uint32_t Frequency;

void Delay_ms(uint16_t ms)
{
	uint32_t temp;
	SysTick->LOAD = ms * 21000;
	SysTick->VAL = 0;
	SysTick->CTRL = 1;
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16)));
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
}

void Delay_us(uint32_t us)
{
	uint32_t temp;
	SysTick->LOAD = us * 21; //168Mhz 8 分频                      
	SysTick->VAL = 0;
	SysTick->CTRL = 1;
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16)));
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
}

void TIM2_PWM_Output_Init(uint16_t Period)
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	TIM_OC_InitTypeDef OC_Config;
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	htim2.Instance = TIM2;							//定时器3
	htim2.Init.Prescaler = 839;						//分频系数
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;	//向上计数模式
	htim2.Init.Period = Period;						//自动重装载值

	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)			//初始化PWM
	{
		_Error_Handler(__FILE__, __LINE__);
	};

	OC_Config.OCMode = TIM_OCMODE_PWM1;
	OC_Config.Pulse = Period >> 1;
	OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	OC_Config.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &OC_Config, TIM_CHANNEL_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void TIM6_DAC_Trigger_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	__HAL_RCC_TIM6_CLK_ENABLE();

	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 7;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 20;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}


void TIM7_Base_Init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig;

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 8399;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 100;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

void TIM7_IRQHandler(void)
{
    if(__HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) !=RESET)
    {
		__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
		Frequency = TIM1->CNT * 800U;
		TIM1->CNT = 0U;
    }
}


void TIM14_PWM_Output_Init(uint16_t Period)
{
	__HAL_RCC_TIM14_CLK_ENABLE();

	TIM_OC_InitTypeDef OC_Config;
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_TIM14;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	htim14.Instance = TIM14;
	htim14.Init.Prescaler = 839;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = Period;

	if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	};

	OC_Config.OCMode = TIM_OCMODE_PWM1;
	OC_Config.Pulse = Period >> 1;
	OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	OC_Config.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim14, &OC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}