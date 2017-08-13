#include "sram.h"

SRAM_HandleTypeDef hsram_External;
SRAM_HandleTypeDef hsram_LCD;

void SRAM_Write(uint32_t Addr, uint8_t* pBuffer, uint32_t Count)
{
	while (Count--)
	{
		*((__IO uint8_t*)(FSMC_EXSRAM_BASE + Addr++)) = *(pBuffer++);
	}
}

void SRAM_Read(uint32_t Addr, uint8_t* pBuffer, uint32_t Count)
{
	while (Count--)
	{
		*(pBuffer++) = *((__IO uint8_t*)(FSMC_EXSRAM_BASE + Addr++));
	}
}

void SRAM_Init(void)
{
	HAL_FSMC_Init();

	FSMC_NORSRAM_TimingTypeDef Timing, ExtTiming;

	/* Perform the SRAM1 memory initialization sequence */
	hsram_External.Instance = FSMC_NORSRAM_DEVICE;
	hsram_External.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram_External.Init */
	hsram_External.Init.NSBank = FSMC_NORSRAM_BANK3;
	hsram_External.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram_External.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram_External.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram_External.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram_External.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram_External.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram_External.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram_External.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram_External.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram_External.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram_External.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram_External.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram_External.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 0x00;
	Timing.AddressHoldTime = 0x00;
	Timing.DataSetupTime = 0x08;	//HCLK = 168MHz 6ns * 9 = 54ns
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;


	if (HAL_SRAM_Init(&hsram_External, &Timing, NULL) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/* Perform the TFTLCD SRAM initialization sequence */
	hsram_LCD.Instance = FSMC_NORSRAM_DEVICE;
	hsram_LCD.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram_LCD.Init */
	hsram_LCD.Init.NSBank = FSMC_NORSRAM_BANK4;
	hsram_LCD.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram_LCD.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram_LCD.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram_LCD.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram_LCD.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram_LCD.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram_LCD.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram_LCD.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram_LCD.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram_LCD.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
	hsram_LCD.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram_LCD.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram_LCD.Init.PageSize = FSMC_PAGE_SIZE_NONE;

	/* Timing */
	Timing.AddressSetupTime = 0x0F;
	Timing.AddressHoldTime = 0x00;
	Timing.DataSetupTime = 24;
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	/* ExtTiming */
	ExtTiming.AddressSetupTime = 0x08;
	ExtTiming.AddressHoldTime = 0x00;
	ExtTiming.DataSetupTime = 0x08;	//HCLK = 168MHz об 6ns * 9 = 54ns
	ExtTiming.BusTurnAroundDuration = 0x00;
	ExtTiming.CLKDivision = 0x00;
	ExtTiming.DataLatency = 0x00;
	ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

	if (HAL_SRAM_Init(&hsram_LCD, &Timing, &ExtTiming) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

static void HAL_FSMC_Init(void) 
{
	static _Bool FSMC_Initialized = 0;

	if (FSMC_Initialized) {
		return;
	}
	FSMC_Initialized = 1;

	/* Peripheral clock enable */
	__HAL_RCC_FSMC_CLK_ENABLE();

	/** FSMC GPIO Configuration
	PF0   ------> FSMC_A0
	PF1   ------> FSMC_A1
	PF2   ------> FSMC_A2
	PF3   ------> FSMC_A3
	PF4   ------> FSMC_A4
	PF5   ------> FSMC_A5
	PF12   ------> FSMC_A6
	PF13   ------> FSMC_A7
	PF14   ------> FSMC_A8
	PF15   ------> FSMC_A9
	PG0   ------> FSMC_A10
	PG1   ------> FSMC_A11
	PE7   ------> FSMC_D4
	PE8   ------> FSMC_D5
	PE9   ------> FSMC_D6
	PE10   ------> FSMC_D7
	PE11   ------> FSMC_D8
	PE12   ------> FSMC_D9
	PE13   ------> FSMC_D10
	PE14   ------> FSMC_D11
	PE15   ------> FSMC_D12
	PD8   ------> FSMC_D13
	PD9   ------> FSMC_D14
	PD10   ------> FSMC_D15
	PD11   ------> FSMC_A16
	PD12   ------> FSMC_A17
	PD13   ------> FSMC_A18
	PD14   ------> FSMC_D0
	PD15   ------> FSMC_D1
	PG2   ------> FSMC_A12
	PG3   ------> FSMC_A13
	PG4   ------> FSMC_A14
	PG5   ------> FSMC_A15
	PD0   ------> FSMC_D2
	PD1   ------> FSMC_D3
	PD4   ------> FSMC_NOE
	PD5   ------> FSMC_NWE
	PG10   ------> FSMC_NE3
	PG12   ------> FSMC_NE4
	PE0   ------> FSMC_NBL0
	PE1   ------> FSMC_NBL1
	*/

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
		| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
		| GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
		| GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
		| GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
		| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13
		| GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
		| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}


