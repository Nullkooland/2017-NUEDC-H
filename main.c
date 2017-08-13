#include <stm32f4xx_hal.h>
#include <string.h>
#include "System\TIMER\timer.h"
#include "System\UART\uart.h"
#include "Hardware\LCD\lcd.h"
#include "FatFS\ff.h"
#include <arm_math.h>

static void SystemClock_Config(void);
void _Error_Handler(char*, int);

char SDPath[4];
FATFS fs;

extern UART_HandleTypeDef huart1;
extern uint16_t UART_RxLen;
extern _Bool UART_RxEnd;
extern uint8_t UART_RxData[UART1_RX_BUFFER_SIZE];

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim14;

extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;

extern  __IO int16_t AD7606_adcValue[8];
extern  __IO int16_t AD922X_adcValue[4096];

int main(void)
{
    HAL_Init();
	SystemClock_Config();

	DMA_Init();
	UART1_Init();
	UART3_Init();
	SRAM_Init();
	LCD_Init(0);
	ZLG7290_Init();
//	AD7606_Init();
//	AD922X_Init();
//	DAC904_Init();
	DAC_Init();

	
	if (f_mount(&fs, (TCHAR const*)SDPath, 1) != FR_OK)
	{
		puts("！！SD卡挂载文件系统失败！！！ ");
		//while (1);
	}
	

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


	//TIM14_PWM_Output_Init(500U);
	//HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

	//LCD_DrawPicture_SD(0, 0, 800, 480, "0:WBB.rgb16");
	//LCD_ShowString(240, 420, 32, "你娃是猪儿虫吗！", WHITE);

	//AD9959_SingleOutput(0x80, 40000000, 0.0f, 1023);

	FreqSweep_Init();
	FreqSweep_Function();

	uint16_t DAC_Value = 3125;
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_Value);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_UART_Receive_DMA(&huart1, &UART_RxData, UART1_RX_BUFFER_SIZE);

	for (;;)
	{
		if (UART_RxEnd)
		{
			//HAL_UART_Transmit_DMA(&huart1, &UART_RxData, UART_RxLen); //双向串口通信测试
			UART_RxData[UART_RxLen] = 0;

			/*
			switch (RxData[0])
			{
			case 'A': HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2); break;
			case 'B': HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2); break;
			case '+':
				if (dutyCycle < 50)
				{
					dutyCycle++;
					__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, dutyCycle);
				}
				break;

			case '-':
				if (dutyCycle >= 1)
				{
					dutyCycle--;
					__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, dutyCycle);
				}
			}*/

			/*
			switch (UART_RxData[0]) 
			{
				case 'P':
				PE4302_SetLoss(0, atoi(UART_RxData + 2));
				break;

				case 'A':
				AD9959_SetAmp(0x80, atoi(UART_RxData + 2));
				break;

				case 'F':
				AD9959_SetFreq(0x80, 1000U * atoi(UART_RxData + 2));
				break;
			}*/

			uint16_t Volt = atoi(UART_RxData) * 1.26f;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Volt);
			

			UART_RxEnd = 0; 
			UART_RxLen = 0;
		}

		/*
		if (ZLG7290_ReadKey() == 1)
		{
			PE4302_SetLoss(GetInterger());
		}*/
		
		GPIOF->BSRR = GPIO_PIN_10;
		Delay_ms(100);
		GPIOF->BSRR = (uint32_t)GPIO_PIN_10 << 16;
		Delay_ms(100);
	}
}

static void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	 /* Configure the main internal regulator output voltage */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	 /* Initializes the CPU, AHB and APB busses clocks */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/* Configure the Systick */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 8000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
}

void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* MemoryManagement_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	/* BusFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	/* UsageFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	/* SVCall_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
	/* DebugMonitor_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void _Error_Handler(char * file, int line)
{
	printf("Error at %s\n", file);
	while (1);
}

void HardFault_Handler(void)
{
	puts("HARD FAULT!!!");
	while (1);
}
