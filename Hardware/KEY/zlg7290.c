#include "zlg7290.h"

I2C_HandleTypeDef hi2c_ZLG7290;
static _Bool KeyDown;
static uint16_t i;

void ZLG7290_Init(void)
{
	__HAL_RCC_I2C1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	hi2c_ZLG7290.Instance = I2C1;
	hi2c_ZLG7290.Init.ClockSpeed = 100000;
	hi2c_ZLG7290.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c_ZLG7290.Init.OwnAddress1 = 0;
	hi2c_ZLG7290.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c_ZLG7290.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c_ZLG7290.Init.OwnAddress2 = 0;
	hi2c_ZLG7290.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c_ZLG7290.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c_ZLG7290);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 3);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

uint8_t ZLG7290_ReadKey(void)
{
	if (!KeyDown) return 0;
	KeyDown = 0;

	uint8_t data = 0x01;
	HAL_I2C_Master_Transmit(&hi2c_ZLG7290, ZLG7290_WRITE_ADDR, &data, 1, 0xFF);
	HAL_I2C_Master_Receive(&hi2c_ZLG7290, ZLG7290_READ_ADDR, &data, 1, 0xFF);
	return data;
}

void EXTI0_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_0))
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		KeyDown = 1;
	}
}