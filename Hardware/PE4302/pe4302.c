#include "pe4302.h"

extern UART_HandleTypeDef huart3;

#ifndef REMODE_MODE
void PE4302_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}
#endif // !REMODE_MODE

void PE4302_SetLoss(uint8_t channel, uint8_t twoTimes_dB)
{
	twoTimes_dB = (twoTimes_dB > 63) ? 63 : twoTimes_dB; //歪，你衰嫩个多干撒子？！

#ifdef REMODE_MODE

	if (channel > 3) {
		puts("歪，你是不是要搞事情！");
		return;
	}

	static uint8_t buffer[5];
	sprintf(&buffer, "P%u%.2u", channel, twoTimes_dB);
	HAL_UART_Transmit(&huart3, &buffer, 4, 0xFF);
#else
	LATCH_HIGH;
	GPIOC->ODR = twoTimes_dB;
	LATCH_LOW;
#endif

	printf("PE4302_%u号 衰减值设为: %4.1f dB\n", channel, twoTimes_dB * 0.5f);
}