#include "FrequencySweep.h"
#include <math.h>
#include <arm_math.h>
#include "Hardware/LCD/lcd.h"
#include "Hardware/KEY/zlg7290.h"
#include "Hardware/DDS/ad9959.h"

static uint8_t PE4302_2xLoss; //dB
static uint8_t Frequency; //MHz

static uint16_t trace_posX;
static uint16_t trace_coveredLine_X[GRID_WIDTH];
static uint16_t trace_coveredLine_Y[GRID_HEIGHT];

//uint16_t axisX_Value[GRID_HEIGHT /50];
//uint16_t axisY_Value[GRID_WIDTH / 50];

static uint16_t adc_Value[100];

uint16_t curve_Value[GRID_WIDTH];
ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart3;

void FreqSweep_Init(void)
{
	/* 硬件外设初始化 */

	PE4302_Init();
	AD9959_Init();
	ADC1_Init();

	PE4302_2xLoss = 0;
	Frequency = 1;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);


	/* GUI 初始化 */
	uint16_t i;
	LCD_Clear(BLACK);
	LCD_DrawRect(GRID_X - 1, GRID_Y - 1, GRID_WIDTH + 2, GRID_HEIGHT + 2, WHITE); //画框框

	for (i = GRID_Y + 10; i < GRID_Y + GRID_HEIGHT; i += 10)		//画细网格-水平 
		LCD_DrawLine(GRID_X, i, GRID_X + GRID_WIDTH, i, DARKGRAY);

	for (i = GRID_X + 10; i < GRID_X + GRID_WIDTH; i += 10)			//画细网格-垂直
		LCD_DrawLine(i, GRID_Y, i, GRID_Y + GRID_HEIGHT, DARKGRAY);

	for (i = GRID_Y + 50; i < GRID_Y + GRID_HEIGHT; i += 50)		//画粗网格-水平
		LCD_DrawLine(GRID_X, i, GRID_X + GRID_WIDTH, i, GRAY);

	for (i = GRID_X + 50; i < GRID_X + GRID_WIDTH; i += 50)			//画粗网格-垂直
		LCD_DrawLine(i, GRID_Y, i, GRID_Y + GRID_HEIGHT, GRAY);


	LCD_ShowString(GRID_X + GRID_WIDTH / 2 - 64, GRID_Y + GRID_HEIGHT + 32, 32, "频率 (MHz)", WHITE);

	LCD_ShowString(GRID_X - 18, GRID_Y + 195, 16, "0", WHITE);

	for (i = 0; i < 3; i++) {
		LCD_ShowNumber(GRID_X - 27, GRID_Y + 45 + i * 50, 16, (3 - i) * 10, WHITE); //纵坐标数值
		LCD_ShowNumber(GRID_X - 27, GRID_Y + 245 + i * 50, 16, (i + 1) * 10, WHITE);
		LCD_ShowString(GRID_X - 35, GRID_Y + 245 + i * 50, 16, "-", WHITE);
	}

	LCD_ShowString(GRID_X - 38, GRID_Y + GRID_HEIGHT - 24, 32, "dB", WHITE);
	
	for (i = 1; i <= 10; i++)
		LCD_ShowNumber(GRID_X - 10 + i * 50, GRID_Y + GRID_HEIGHT + 3, 16, i * 5, WHITE);

	//LCD_DrawLine(550, 0, 550, 480, WHITE);

	for (i = 0; i < 4; i++)
		LCD_DrawRect(550, 9 + i * 103, 245, 93, WHITE);

	LCD_ShowString(610, 13, 32, "运行状态", WHITE);
}

void FreqSweep_Function()
{
	for (;;)
	{
		switch (ZLG7290_ReadKey())
		{
		case 1:
			FreqPoint_Output();
			break;

		case 9:
			FreqSweep_Start();
			break;

		case 17:
			Draw_Curve(LIGHTBLUE);
			Curve_Trace();
			Recover_Grid();
			break;
		}

		GPIOF->BSRR = GPIO_PIN_10;
		Delay_ms(75);
		GPIOF->BSRR = (uint32_t)GPIO_PIN_10 << 16;
		Delay_ms(75);
	}
}

void FreqPoint_Output(void)
{

	Update_StatusDisplay(FREQ_POINT_OUTPUT);
	LCD_ShowNumber(640, 260, 32, Frequency, GREEN);
	LCD_ShowNumber(640, 156, 32, PE4302_2xLoss, RED);

	AD9959_SetAmp(AD9959_CHANNEL_3, 1023);
	AD9959_SetFreq(AD9959_CHANNEL_3, Frequency * 1000000U);
	PE4302_SetLoss(0, PE4302_2xLoss);


	_Bool flag = 1;
	while (flag)
	{
		switch (ZLG7290_ReadKey())
		{
		case 34:
			if (Frequency > 1)
			{
				Frequency--;
				AD9959_SetFreq(AD9959_CHANNEL_3, Frequency * 1000000U);
				LCD_FillRect(640, 260, 32, 32, BLACK);
				LCD_ShowNumber(640, 260, 32, Frequency, GREEN);
			}
			break;

		case 36:
			if (Frequency < 40)
			{
				Frequency++;
				AD9959_SetFreq(AD9959_CHANNEL_3, Frequency * 1000000U);
				LCD_FillRect(640, 260, 32, 32, BLACK);
				LCD_ShowNumber(640, 260, 32, Frequency, GREEN);
			}
			break;

		case 37:
			if (PE4302_2xLoss < 63)
			{
				PE4302_SetLoss(0, ++PE4302_2xLoss);
				LCD_FillRect(640, 156, 48, 32, BLACK);
				LCD_ShowNumber(640, 156, 32, PE4302_2xLoss, RED);
			}
			break;

		case 29:
			if (PE4302_2xLoss > 0)
			{
				PE4302_SetLoss(0, --PE4302_2xLoss);
				LCD_FillRect(640, 156, 48, 32, BLACK);
				LCD_ShowNumber(640, 156, 32, PE4302_2xLoss, RED);
			}
			break;

		case 5:
			flag = 0;
			Update_StatusDisplay(CHI_GUA);
			break;
		}

		Delay_ms(10);
	}
}


void FreqSweep_Start(void)
{
	Update_StatusDisplay(LINEAR_FREQ_SWEEP);

	LCD_ShowNumber(640, 156, 32, 50, RED);
	LCD_ShowString(700, 155, 32, "mV", GRAY);

	LCD_ShowString(630, 260, 32, "0.5", GREEN);

	AD9959_SetAmp(AD9959_CHANNEL_3, 1023);
	PE4302_SetLoss(0, 25);

	uint16_t adc_SingleResult;
	HAL_ADC_Start_DMA(&hadc1, &adc_SingleResult, 1);

	for (;;) {

		TRRIGER_HIGH;
		TRRIGER_LOW;
		
		for (uint16_t i = 1; i <= 100; i++)
		{
			AD9959_SetFreq(AD9959_CHANNEL_3, i * 500000U);
			Delay_us(200);
			adc_Value[i - 1] = adc_SingleResult;

			if (ZLG7290_ReadKey() == 5)
			{
				Update_StatusDisplay(CHI_GUA);
				HAL_ADC_Stop_DMA(&hadc1);
				return;
			}
		}

		HAL_UART_Transmit(&huart3, &adc_Value, 200, 0xFF);

		for (uint16_t i = 0; i < 100; i++) {
			curve_Value[i * 5] = 200 - ((2147 - adc_Value[i]) / 31 * 5);
		}
		Draw_Curve(RED);
		Delay_ms(50);
		Recover_Grid();
	}

}

void FreqSweep_Test(void)
{
	FreqSweep_Start();

	uint16_t i;
	float delta = 0.0001f;
	for (;;)
	{
		float value = -3.1416f;
		for (i = 0; i < GRID_WIDTH; i++)
		{
			curve_Value[i] = (1.5f + arm_sin_f32(value)) * 150U;
			value += delta;
		}

		Draw_Curve(LIGHTBLUE);

		delta += 0.0001f;

		if (delta > 0.05f) delta = 0.0001f;

		if (ZLG7290_ReadKey() == 1)	Curve_Trace();

		Delay_ms(50);
		Recover_Grid();
	}
}

void Curve_Trace(void)
{
	Update_StatusDisplay(CURVE_TRACE);
	LCD_ShowNumber(640, 156, 32, 50, RED);
	LCD_ShowString(700, 155, 32, "mV", GRAY);

	LCD_ShowString(690, 363, 32, "dB", GRAY);

	trace_posX = GRID_WIDTH / 2;
	Curve_Trace_DrawLine();

	_Bool flag = 1;

	while (flag)
	{
		switch (ZLG7290_ReadKey())
		{
		case 34:

			if (trace_posX > 0) {
				Curve_Trace_RecoverLine();
				trace_posX -= 10;
				Curve_Trace_DrawLine();
			}
			break;

		case 36:

			if (trace_posX < GRID_WIDTH - 10) {
				Curve_Trace_RecoverLine();
				trace_posX += 10;
				Curve_Trace_DrawLine();
			}
			break;

		case 5: 
			flag = 0;
			Curve_Trace_RecoverLine();
			Update_StatusDisplay(CHI_GUA);
			break;
		}
		Delay_ms(20);
	}
}

static void Update_StatusDisplay(uint8_t status)
{
	LCD_FillRect(610, 54, 128, 32, BLACK);
	LCD_ShowString(610, 54, 32, StatusTag[status], GBLUE);

	for (uint8_t i = 0; i < 3; i++)
	{
		LCD_FillRect(610, 116 + i * 103, 128, 32, BLACK);
		LCD_ShowString(610, 116 + i * 103, 32, StatusLabel[status][i], WHITE);
	}

	if (status == CHI_GUA) {
		LCD_FillRect(630, 155, 128, 32, BLACK);
		LCD_FillRect(630, 260, 128, 32, BLACK);
		LCD_FillRect(630, 363, 128, 32, BLACK);
	}
	else {
		LCD_ShowString(690, 260, 32, "MHz", GRAY);
	}
}

static void Curve_Trace_DrawLine(void)
{
	uint16_t i;

	for (i = 0; i < GRID_HEIGHT; i++)
		trace_coveredLine_Y[i] = LCD_ReadPoint(GRID_X + trace_posX, GRID_Y + i);

	for (i = 0; i < GRID_WIDTH; i++)
		trace_coveredLine_X[i] = LCD_ReadPoint(GRID_X + i, GRID_Y + GRID_HEIGHT - curve_Value[trace_posX]);

	for (i = GRID_Y; i < GRID_Y + GRID_HEIGHT; i++)
		LCD_DrawPixel(GRID_X + trace_posX, i, YELLOW);

	for (i = GRID_X; i < GRID_X + GRID_WIDTH; i++)
		LCD_DrawPixel(i, GRID_Y + GRID_HEIGHT - curve_Value[trace_posX], YELLOW);

	LCD_ShowNumber(640, 260, 32, trace_posX / 10, GREEN);

	if (curve_Value[trace_posX] < 200)
	{
		LCD_ShowString(640, 363, 32, "-", YELLOW);
		LCD_ShowNumber(655, 363, 32, (200 - curve_Value[trace_posX]) / 5, YELLOW);
	}
	else
	{
		LCD_ShowNumber(640, 363, 32, (curve_Value[trace_posX] - 200) / 5, YELLOW);
	}

}

static void Curve_Trace_RecoverLine(void)
{
	uint16_t i;

	for (i = 0; i < GRID_HEIGHT; i++)
		LCD_DrawPixel(GRID_X + trace_posX, GRID_Y + i, trace_coveredLine_Y[i]);

	for (i = 0; i < GRID_WIDTH; i++)
		LCD_DrawPixel(GRID_X + i, GRID_Y + GRID_HEIGHT - curve_Value[trace_posX], trace_coveredLine_X[i]);

	LCD_FillRect(640, 260, 32, 32, BLACK);
	LCD_FillRect(640, 363, 48, 32, BLACK);
}

static inline void Recover_Grid(void)
{
	for (uint16_t i = 0; i < GRID_WIDTH; i++)
	{
		Recover_GridPixel(i, curve_Value[i]);
		Recover_GridPixel(i + 1, curve_Value[i]);
		Recover_GridPixel(i - 1, curve_Value[i]);
		Recover_GridPixel(i, curve_Value[i] + 1);
		Recover_GridPixel(i, curve_Value[i] - 1);
	}
}

static inline void Recover_GridPixel(uint16_t x0, uint16_t y0)
{
	if ((x0 % 50 == 0) || (y0 % 50 == 0)) {
		LCD_DrawPixel(GRID_X + x0, GRID_Y + GRID_HEIGHT - y0, GRAY);
		return;
	}

	if ((x0 % 10 == 0) || (y0 % 10 == 0)) {
		LCD_DrawPixel(GRID_X + x0, GRID_Y + GRID_HEIGHT - y0, DARKGRAY);
		return;
	}

	LCD_DrawPixel(GRID_X + x0, GRID_Y + GRID_HEIGHT - y0, BLACK);
}

static inline void Draw_Curve(uint16_t Color)
{
	uint16_t x, y;
	for (uint16_t i = 0; i < GRID_WIDTH; i++)
	{
		if(curve_Value[i] == 0) continue;

		x = GRID_X + i;
		y = GRID_Y + GRID_HEIGHT - curve_Value[i];
		LCD_DrawPixel(x, y, Color);
		LCD_DrawPixel(x + 1, y, Color);
		LCD_DrawPixel(x - 1, y, Color);
		LCD_DrawPixel(x, y + 1, Color);
		LCD_DrawPixel(x, y - 1, Color);
	}
}



