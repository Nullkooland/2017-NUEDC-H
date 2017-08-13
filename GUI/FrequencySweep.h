#pragma once
#include <stm32f4xx_hal.h>

#define TRRIGER_HIGH	GPIOG->BSRR = GPIO_PIN_6
#define TRRIGER_LOW		GPIOG->BSRR = (uint32_t)GPIO_PIN_6	<< 16U

#define GRID_X			40
#define GRID_Y			10
#define GRID_WIDTH		500
#define GRID_HEIGHT		400

static const uint8_t* StatusTag[4] = { "点频输出", "线性扫频", "幅频曲线","等待指令" };

static const uint8_t* StatusLabel[4][3] = {
	{ "衰减等级", "输出频率", "" },
	{ "参考电平", "频率步进", "" },
	{ "参考电平", "频率点", "增益量" },
	{ "", "", "" },
};

typedef enum {
	FREQ_POINT_OUTPUT, LINEAR_FREQ_SWEEP, CURVE_TRACE, CHI_GUA
} RUN_STATUS;


void FreqSweep_Init(void);
void FreqSweep_Test(void);

void FreqSweep_Start(void);
void FreqPoint_Output(void);


void Curve_Trace(void);
static void Curve_Trace_DrawLine(void);
static void Curve_Trace_RecoverLine(void);

static void Update_StatusDisplay(uint8_t status);
static inline void Draw_Curve(uint16_t Color);
static inline void Recover_Grid(void);
static inline void Recover_GridPixel(uint16_t x0, uint16_t y0);
