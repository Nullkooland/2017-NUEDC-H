#include "ad9833.h"

union
{
	uint16_t VALUE;		//START WITH DB0 TO DB15
	struct {
		_Bool			: 1;
		_Bool MODE		: 1;
		_Bool			: 1;
		_Bool DIV2		: 1;
		_Bool			: 1;
		_Bool OPBITEN	: 1;
		_Bool SLEEP12	: 1;
		_Bool SLEEP1	: 1;
		_Bool RESET		: 1;
		_Bool			: 1;
		_Bool PSEL		: 1;
		_Bool FSEL		: 1;
		_Bool HLB		: 1;
		_Bool B28		: 1;
		uint16_t		: 2;
	} BITS;
} Com_Data;

union
{
	uint16_t VALUE;
	struct {
		uint16_t REG	: 14;
		uint16_t ADDR	: 2;
	} BITS;
} Freq_Data;


void AD9833_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	Com_Data.VALUE = 0x0000;

	Com_Data.BITS.B28 = 1;
	Com_Data.BITS.FSEL = 0;
	Com_Data.BITS.PSEL = 0;
	//	Com_Data.BITS.OPBITEN = 1;
	Com_Data.BITS.MODE = 0;

	Delay_ms(5);
	WriteReg(Com_Data.VALUE);

	WriteReg(0xC000); //ÏàÎ»¼Ä´æÆ÷ÖÃÁã
	AD9833_WriteFreq(1000);
}


void AD9833_WriteFreq(uint32_t freq) //HZ
{
	freq *= 22.369621f;

	Freq_Data.BITS.ADDR = 0x01;

	Freq_Data.BITS.REG = freq & 0x3FFF;
	WriteReg(Freq_Data.VALUE);

	Freq_Data.BITS.REG = (freq & 0x0FFFC000) >> 14;
	WriteReg(Freq_Data.VALUE);
}

void AD9833_SwitchMode(void)
{
	Com_Data.BITS.MODE = !Com_Data.BITS.MODE;
	WriteReg(Com_Data.VALUE);
}

static void WriteReg(uint16_t data)
{
	FSYNC0;
	for (uint8_t i = 0; i < 16; ++i)
	{
		if (data & 0x8000) SDATA1;
		else SDATA0;

		SCLK1;
		Delay_us(1);
		SCLK0;
		Delay_us(1);
		data <<= 1;
	}

	SCLK1;
	FSYNC1;
}