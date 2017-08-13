#include "spi.h"

SPI_HandleTypeDef hspi2;
//extern DMA_HandleTypeDef hdma_spi2_rx;
//extern DMA_HandleTypeDef hdma_spi2_tx;

void SPI2_Init(void)
{
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	if (hspi->Instance == SPI2)
	{
		__HAL_RCC_SPI2_CLK_ENABLE();

		/* SPI2 GPIO Configuration
		PB13     ------> SPI2_SCK
		PB14     ------> SPI2_MISO
		PB15     ------> SPI2_MOSI
		*/
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*
		hdma_spi2_rx.Instance = DMA1_Stream3;
		hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
		hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_spi2_rx.Init.Mode = DMA_NORMAL;
		hdma_spi2_rx.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(hspi, hdmarx, hdma_spi2_rx);

		hdma_spi2_tx.Instance = DMA1_Stream4;
		hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
		hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_spi2_tx.Init.MemInc = DMA_MINC_DISABLE;
		hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_spi2_tx.Init.Mode = DMA_NORMAL;
		hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(hspi, hdmatx, hdma_spi2_tx);*/
	}

}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
	if (hspi->Instance == SPI2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_SPI2_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

		/* SPI1 DMA DeInit */
		//HAL_DMA_DeInit(hspi->hdmarx);
	}

}

void SPI2_SetSpeed(uint16_t BaudRatePrescaler)
{
	SPI2->CR1 = ((uint16_t)(SPI2->CR1 & 0xFFC7)) | (BaudRatePrescaler & 0x0038);
	SPI2->CR1 |= SPI_CR1_SPE;
}

uint8_t SPI2_RW_Byte(uint8_t data)
{
	while (!(SPI2->SR & SPI_FLAG_TXE));
	SPI2->DR = data;
	while (!(SPI2->SR & SPI_FLAG_RXNE));
	return SPI2->DR;
}
