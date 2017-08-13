#include "uart.h"

UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_tx;

UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_uart1_rx;
extern DMA_HandleTypeDef hdma_uart1_tx;


uint16_t UART_RxLen;
_Bool UART_RxEnd;
uint8_t UART_RxData[UART1_RX_BUFFER_SIZE];

size_t _write(int file, const uint8_t *pBuffer, size_t len) //重定向输出流到串口DMA
{
	HAL_UART_Transmit_DMA(&huart1, pBuffer, len);
	return len;
}

void UART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void UART3_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART1)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		hdma_uart1_rx.Instance = DMA2_Stream2;
		hdma_uart1_rx.Init.Channel = DMA_CHANNEL_4;
		hdma_uart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_uart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_uart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_uart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_uart1_rx.Init.Mode = DMA_CIRCULAR;
		hdma_uart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_uart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_uart1_rx) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(huart, hdmarx, hdma_uart1_rx);

		hdma_uart1_tx.Instance = DMA2_Stream7;
		hdma_uart1_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_uart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_uart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_uart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_uart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_uart1_tx.Init.Mode = DMA_NORMAL;
		hdma_uart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_uart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_uart1_tx) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(huart, hdmatx, hdma_uart1_tx);

		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE | UART_IT_TXE);
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	else if (huart->Instance == USART3)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART3_CLK_ENABLE();

		/**USART3 GPIO Configuration
		PB10     ------> USART3_TX
		*/
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART3 DMA Init */
		/* USART3_TX Init */
		hdma_usart3_tx.Instance = DMA1_Stream3;
		hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart3_tx.Init.Mode = DMA_NORMAL;
		hdma_usart3_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
		hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(huart, hdmatx, hdma_usart3_tx);
	}

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART1)
	{
		__HAL_RCC_USART1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
		HAL_DMA_DeInit(huart->hdmarx);
		HAL_DMA_DeInit(huart->hdmatx);
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
	else if (huart->Instance == USART3)
	{
		__HAL_RCC_USART3_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
		HAL_DMA_DeInit(huart->hdmatx);
	}
}

void USART1_IRQHandler(void)
{
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);

		__HAL_DMA_DISABLE(huart1.hdmarx);
		UART_RxLen = UART1_RX_BUFFER_SIZE - hdma_uart1_rx.Instance->NDTR;
		__HAL_DMA_ENABLE(huart1.hdmarx);

		UART_RxEnd = 1;
	}
	HAL_UART_IRQHandler(&huart1);
}

