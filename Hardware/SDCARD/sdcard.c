#include "sdcard.h"
#include "System/SPI/spi.h"

SD_CARDINFO SD_CardInfo;
extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;

const uint8_t Dummy_Byte = 0xFF;

SD_ERROR SD_Init(void)
{

	//if ((SD_CardInfo.Status & STA_NOINIT) == 0x00) return SD_NO_ERROR;

	SD_ERROR result;
	uint32_t i;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	SPI2_Init();
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_256);
	__HAL_SPI_ENABLE(&hspi2);

	SD_CardInfo.Status = STA_NOINIT;
	SD_CardInfo.Type = SD_TYPE_UNKNOW;

	SD_CS1;
	for (i = 0; i < 10; i++)
	{
		SPI2_RW_Byte(Dummy_Byte);
	}

	if (SD_SendCmd(0, 0, 0x95) != SD_IN_IDLE_STATE) return SD_RESPONSE_FAILURE;

	SPI2_RW_Byte(Dummy_Byte);
	SPI2_RW_Byte(Dummy_Byte);

	result = SD_SendCmd(8, 0x01AA, 0x87);

	if (result == SD_IN_IDLE_STATE) // SDv2 Card or later
	{
		uint8_t ocr[4]; //OCR (Operation Conditions Register)
		ocr[0] = SPI2_RW_Byte(Dummy_Byte);
		ocr[1] = SPI2_RW_Byte(Dummy_Byte);
		ocr[2] = SPI2_RW_Byte(Dummy_Byte);
		ocr[3] = SPI2_RW_Byte(Dummy_Byte);

		if (ocr[2] == 0x01 && ocr[3] == 0xAA) //电压范围2.7V~3.6V
		{
			i = 40000;
			do
			{
				result = SD_SendCmd(55, 0, Dummy_Byte); // CMD55
				if (result == SD_IN_IDLE_STATE)
				{
					result = SD_SendCmd(41, 0x40000000, Dummy_Byte); // ACMD41 注意参数
				}
			} while ((result != SD_NO_ERROR) && --i);

			if (i)
			{
				// CMD58 读取 OCR
				result = SD_SendCmd(58, 0, Dummy_Byte);
				if (result == SD_NO_ERROR)
				{
					ocr[0] = SPI2_RW_Byte(Dummy_Byte);
					ocr[1] = SPI2_RW_Byte(Dummy_Byte);
					ocr[2] = SPI2_RW_Byte(Dummy_Byte);
					ocr[3] = SPI2_RW_Byte(Dummy_Byte);
					if (ocr[0] & 0x40) // CCS bit (Card Capacity Status)
					{
						SD_CardInfo.Type = SD_TYPE_SDHC; // or SDXC
					}
					else
					{
						SD_CardInfo.Type = SD_TYPE_SDV2;
					}
				}
			}
		}
	}
	else
	{
		i = 40000;
		do // 初始化
		{
			result = SD_SendCmd(55, 0, Dummy_Byte); // CMD55
			if (result == SD_IN_IDLE_STATE)
			{
				result = SD_SendCmd(41, 0x00, Dummy_Byte); // ACMD41 参数与 SDv2 不同
			}
		} while ((result != SD_NO_ERROR) && --i);
		if (i) // 有回应是 SDv1, 且初始化完成
		{
			SD_CardInfo.Type = SD_TYPE_SDV1;
		}
		else // 没有回应是 MMC
		{
			i = 40000;
			do // 初始化
			{
				result = SD_SendCmd(1, 0x00, Dummy_Byte); //CMD1 参数与 SDv2 不同
			} while ((result != SD_NO_ERROR) && --i);
			if (i) SD_CardInfo.Type = SD_TYPE_MMC3;
		}
	}

	if (SD_CardInfo.Type != SD_TYPE_UNKNOW)
	{
		SD_CardInfo.Status &= ~STA_NOINIT; //初始化成功
										   // CMD59 关闭 CRC 校验
		SD_SendCmd(59, 0x00, Dummy_Byte);
		// CMD16 设置 BLOCK 为 512 Bytes (对 SDHC/SDHX 无效)
		SD_SendCmd(16, 0x200, Dummy_Byte);

		result = SD_NO_ERROR;
	}
	else
	{
		result = SD_RESPONSE_FAILURE;
	}
	
	SD_CS1;
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_2);
	SD_GetCardInfo();
	return result;

}

SD_ERROR SD_GetCardInfo(void)
{
	SD_ERROR result;
	uint8_t CSD_Tab[16];
	uint8_t CID_Tab[16];
	uint32_t i;

	/* Send CMD9, Read CSD */
	result = SD_SendCmd(9, 0, 0xFF);

	if (result == SD_NO_ERROR)
	{
		i = 10;
		while (SPI2_RW_Byte(Dummy_Byte) != 0xFE && --i);

		if (i > 0)
		{
			for (i = 0; i < 16; i++) // Store CSD register value on CSD_Tab
			{
				CSD_Tab[i] = SPI2_RW_Byte(Dummy_Byte);
			}
			// Get CRC bytes (not really needed by ME, but required by SD)
			SPI2_RW_Byte(Dummy_Byte);
			SPI2_RW_Byte(Dummy_Byte);
		}
	}
	SD_CS1;
	SPI2_RW_Byte(Dummy_Byte);

	SD_CardInfo.CSD.CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
	SD_CardInfo.CSD.SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
	SD_CardInfo.CSD.Reserved1 = CSD_Tab[0] & 0x03;

	SD_CardInfo.CSD.TAAC = CSD_Tab[1];

	SD_CardInfo.CSD.NSAC = CSD_Tab[2];

	SD_CardInfo.CSD.MaxBusClkFrec = CSD_Tab[3];

	SD_CardInfo.CSD.CardComdClasses = CSD_Tab[4] << 4;

	SD_CardInfo.CSD.CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
	SD_CardInfo.CSD.RdBlockLen = CSD_Tab[5] & 0x0F;

	SD_CardInfo.CSD.PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
	SD_CardInfo.CSD.WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
	SD_CardInfo.CSD.RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
	SD_CardInfo.CSD.DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
	SD_CardInfo.CSD.Reserved2 = 0;
	SD_CardInfo.CSD.DeviceSize = (CSD_Tab[6] & 0x03) << 10;

	SD_CardInfo.CSD.DeviceSize |= (CSD_Tab[7]) << 2;

	SD_CardInfo.CSD.DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
	SD_CardInfo.CSD.MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
	SD_CardInfo.CSD.MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

	SD_CardInfo.CSD.MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
	SD_CardInfo.CSD.MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
	SD_CardInfo.CSD.DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;


	SD_CardInfo.CSD.DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
	SD_CardInfo.CSD.EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
	SD_CardInfo.CSD.EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;


	SD_CardInfo.CSD.EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
	SD_CardInfo.CSD.WrProtectGrSize = (CSD_Tab[11] & 0x7F);

	SD_CardInfo.CSD.WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
	SD_CardInfo.CSD.ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
	SD_CardInfo.CSD.WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
	SD_CardInfo.CSD.MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

	SD_CardInfo.CSD.MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
	SD_CardInfo.CSD.WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
	SD_CardInfo.CSD.Reserved3 = 0;
	SD_CardInfo.CSD.ContentProtectAppli = (CSD_Tab[13] & 0x01);


	SD_CardInfo.CSD.FileFormatGroup = (CSD_Tab[14] & 0x80) >> 7;
	SD_CardInfo.CSD.CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
	SD_CardInfo.CSD.PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
	SD_CardInfo.CSD.TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
	SD_CardInfo.CSD.FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
	SD_CardInfo.CSD.ECC = (CSD_Tab[14] & 0x03);

	/*!< Byte 15 */
	SD_CardInfo.CSD.CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
	SD_CardInfo.CSD.Reserved4 = 1;

	SD_CardInfo.Capacity = (SD_CardInfo.CSD.DeviceSize + 1);
	SD_CardInfo.Capacity *= (1 << (SD_CardInfo.CSD.DeviceSizeMul + 2));
	SD_CardInfo.BlockSize = 1 << (SD_CardInfo.CSD.RdBlockLen);
	SD_CardInfo.Capacity *= SD_CardInfo.BlockSize;

	return result;
}

SD_ERROR SD_ReadSingleBlock(uint32_t Addr, uint8_t* pBuffer, uint16_t BlockSize)
{
	SD_ERROR result;

	// Send CMD17 to read single block
	result = SD_SendCmd(17, Addr, Dummy_Byte);
	if (result == SD_NO_ERROR)
	{
		// Now look for the data token(0xFE) to signify the start of the data
		uint32_t i = 200000U; //200ms
		while ((SPI2_RW_Byte(Dummy_Byte) != 0xFE) && --i);

		if (i > 0)
		{
			//hspi2.hdmatx->Instance->CR &= ~DMA_MINC_ENABLE;
			//HAL_SPI_TransmitReceive_DMA(&hspi2, &Dummy_Byte, pBuffer, BlockSize);
			//HAL_DMA_PollForTransfer(&hdma_spi2_rx, HAL_DMA_FULL_TRANSFER, 0xFF);
	
			for (uint16_t i = 0; i < BlockSize; i++)
			{
				pBuffer[i] = SPI2_RW_Byte(Dummy_Byte);
			}

			// Get CRC bytes (not really needed by ME, but required by SD, fuck you)
			SPI2_RW_Byte(Dummy_Byte);
			SPI2_RW_Byte(Dummy_Byte);
			// Set response value to success
			result = SD_NO_ERROR;
		}
		else
		{
			result = SD_RESPONSE_FAILURE;
		}
	}

	SD_CS1;
	SPI2_RW_Byte(Dummy_Byte);
	return result;
}


SD_ERROR SD_ReadMultiBlocks(uint32_t Addr, uint8_t* pBuffer, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	SD_ERROR result;

	// Send CMD18 to read multiple blocks
	result = SD_SendCmd(18, Addr, 0xFF);

	if (result == SD_NO_ERROR)
	{
		hspi2.hdmatx->Instance->CR &= ~DMA_MINC_ENABLE;

		for (uint32_t i = 0; i < NumberOfBlocks; i++)
		{
			// Now look for the data token(0xFE) to signify the start of the data
			uint32_t j = 200000U; //200ms
			while ((SPI2_RW_Byte(Dummy_Byte) != 0xFE) && --j);

			if (j > 0)
			{
				//HAL_SPI_TransmitReceive_DMA(&hspi2, &Dummy_Byte, pBuffer + i * BlockSize, BlockSize);

				for (j = 0; j < BlockSize; j++)
				{
					pBuffer[i*BlockSize + j] = SPI2_RW_Byte(Dummy_Byte);
				}

				// Get CRC bytes (not really needed by us, but required by SD)
				SPI2_RW_Byte(Dummy_Byte);
				SPI2_RW_Byte(Dummy_Byte);
				// Set response value to success
				result = SD_NO_ERROR;
			}
			else
			{
				result = SD_RESPONSE_FAILURE; // Set response value to failure
			}
		}
		// CMD12 停止传输
		result = SD_SendCmd(12, 0, 0xFF);

	}

	SD_CS1;
	SPI2_RW_Byte(Dummy_Byte);
	return result;
}

SD_ERROR SD_WriteSingleBlock(uint32_t Addr, uint8_t* pBuffer, uint16_t BlockSize)
{
	SD_ERROR result;
	// Send CMD24 to write single block
	result = SD_SendCmd(24, Addr, 0xFF);

	if (result == SD_NO_ERROR)
	{
		if (SD_WaitForReady() == SD_NO_ERROR)
		{
			hspi2.hdmatx->Instance->CR |= DMA_MINC_ENABLE;
			// Send the data token(0xFE) to signify the start of the data
			SPI2_RW_Byte(0xFE);

			//HAL_SPI_Transmit_DMA(&hspi2, pBuffer, BlockSize);

			for (uint16_t i = 0; i < BlockSize; i++)
			{
				SPI2_RW_Byte(pBuffer[i]);
			}

			// Put CRC bytes (not really needed by us, but required by SD)
			SPI2_RW_Byte(Dummy_Byte);
			SPI2_RW_Byte(Dummy_Byte);

			// 读取数据响应
			if ((SPI2_RW_Byte(Dummy_Byte) & 0x15) == SD_DATA_OK)
			{
				result = SD_NO_ERROR;
			}
			else
			{
				result = SD_RESPONSE_FAILURE;
			}
		}
	}
	else
	{	// 有时 SD 卡已发送响应，但单片机的 SPI 无法正确解析。
		// 此时 SD 卡处于接收数据状态，很可能将未知数据写入 SD 卡。
		// 为保险起见，强制发送 CMD12 停止传输，使 SD 卡重新进入传输状态。
		SD_SendCmd(12, 0x00, 0xFF);
		SPI2_RW_Byte(Dummy_Byte);
	}

	SD_CS1;
	SPI2_RW_Byte(Dummy_Byte);
	return result;
}

SD_ERROR SD_WriteMultiBlocks(uint32_t Addr, uint8_t* pBuffer, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	SD_ERROR result;
	uint32_t i;

	result = SD_SendCmd(55, 0x00, 0xFF);
	if (SD_SendCmd(55, 0x00, 0xFF) == SD_NO_ERROR)
	{
		SD_SendCmd(23, NumberOfBlocks, 0xFF);
	}
	else return SD_ERASE_SEQUENCE_ERROR;

	result = SD_SendCmd(25, Addr, 0xFF);

	if (result == SD_NO_ERROR)
	{
		hspi2.hdmatx->Instance->CR |= DMA_MINC_ENABLE;

		for (i = 0; i < NumberOfBlocks; i++)
		{
			if (SD_WaitForReady() != SD_NO_ERROR) break;

			SPI2_RW_Byte(0xFC);

			for (uint16_t j = 0; j < BlockSize; j++)
			{
				SPI2_RW_Byte(pBuffer[j + i * BlockSize]);
			}

			// Put CRC bytes (not really needed by us, but required by SD)
			SPI2_RW_Byte(Dummy_Byte);
			SPI2_RW_Byte(Dummy_Byte);

			if ((SPI2_RW_Byte(Dummy_Byte) & 0x1F) != SD_DATA_OK) break;
		}
		if (i < NumberOfBlocks)
		{
			result = SD_DATA_WRITE_ERROR;
		}
		else if (SD_WaitForReady() != SD_NO_ERROR)
		{
			result = SD_RESPONSE_FAILURE;
		}
		SPI2_RW_Byte(0xFD);
	}
	else
	{
		SD_WaitForReady();
		SPI2_RW_Byte(0xFD);
		SD_SendCmd(12, 0x00, 0xFF);
	}

	SD_CS1;
	SPI2_RW_Byte(Dummy_Byte);
	return result;
}

static SD_ERROR SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	SD_CS0;
	if (SD_WaitForReady() == SD_RESPONSE_FAILURE)
	{
		SD_CS1;
		SPI2_RW_Byte(Dummy_Byte);
		return SD_RESPONSE_FAILURE;
	}

	SPI2_RW_Byte(cmd | 0x40);
	SPI2_RW_Byte(arg >> 24);
	SPI2_RW_Byte(arg >> 16);
	SPI2_RW_Byte(arg >> 8);
	SPI2_RW_Byte(arg);
	SPI2_RW_Byte(crc);

	uint8_t i = 10;
	SD_ERROR result;

	do {
		result = (SD_ERROR)SPI2_RW_Byte(Dummy_Byte);
	} while ((result & 0x80) && --i);
	
	return result;
}

SD_ERROR SD_WaitForReady(void)
{
	uint32_t i = 50000U;
	while (--i) {
		if(SPI2_RW_Byte(Dummy_Byte) == 0xFF) return SD_NO_ERROR;
	}
	return SD_RESPONSE_FAILURE;
}
