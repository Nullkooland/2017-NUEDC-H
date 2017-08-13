#include <stm32f4xx_hal.h>
#include <string.h>
#include "Hardware/KEY/zlg7290.h"
#include "Hardware/LCD/lcd.h"

static uint8_t offset;
static _Bool DotInputed;
static uint8_t Input[12];

static void DotInput(void)
{
	if (DotInputed || offset == 0) return;

	LCD_FillRect(offset << 5, 88, 32, 4, BLACK);
	LCD_ShowBigNumber(offset << 5, 32, 10, WHITE);
	Input[offset++] = '.';
	LCD_FillRect(offset << 5, 88, 32, 4, WHITE);

	DotInputed = 1;
}

static void NumberInput(uint8_t num)
{
	if (offset >= 10) return;

	LCD_FillRect(offset << 5, 88, 32, 4, BLACK);
	LCD_ShowBigNumber(offset << 5, 32, num, WHITE);
	Input[offset++] = num + '0';
	LCD_FillRect(offset << 5, 88, 32, 4, WHITE);
}

static void BackSpace(void)
{
	if (Input[offset] == '.') DotInputed = 0;

	if (offset == 0) return;

	LCD_FillRect(offset << 5, 88, 32, 4, BLACK);
	offset--;
	LCD_FillRect(offset << 5, 32, 32, 64, BLACK);
	LCD_FillRect(offset << 5, 88, 32, 4, WHITE);
}

static _Bool GetUserInput(void)
{
	offset = 0;
	LCD_Clear(BLACK);
	LCD_ShowString(0, 0, 32, "请输入数值:", RED);
	LCD_FillRect(0, 88, 32, 4, WHITE);

	for (;;)
	{
		switch (ZLG7290_ReadKey())
		{
			case 2: NumberInput(1); break;
			case 3: NumberInput(2); break;
			case 4: NumberInput(3); break;
			case 10: NumberInput(4); break;
			case 11: NumberInput(5); break;
			case 12: NumberInput(6); break;
			case 18: NumberInput(7); break;
			case 19: NumberInput(8); break;
			case 20: NumberInput(9); break;
			case 27: NumberInput(0); break;
			case 28: DotInput(); break;

			case 5: BackSpace(); break;	//退格

			case 13: LCD_FillRect(offset << 5, 88, 32, 4, BLACK);  //取消输入
				return 0;						

			case 21: 
				Input[offset] = 0; 
				LCD_FillRect(offset << 5, 88, 32, 4, BLACK); //输入结束
				return 1;	

			break;
		};
		Delay_ms(100);
	}

}

uint32_t GetInterger(void)
{
	DotInputed = 1; //获取整数不允许输入小数点

	if (GetUserInput())
	{
		LCD_ShowString(0, 100, 32, "输入成功!", GREEN);
		return atoi(Input);
	}
	LCD_ShowString(0, 100, 32, "已取消输入!", GRAYBLUE);
	return  0;
}

float GetFloat(void)
{
	DotInputed = 0;

	if (GetUserInput())
	{
		LCD_ShowString(0, 100, 32, "输入成功!", GREEN);
		return atof(Input);
	}

	LCD_ShowString(0, 100, 32, "已取消输入!", GRAYBLUE);
	return 0.0f;
}