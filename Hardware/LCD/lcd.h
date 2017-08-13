#pragma once
#include <stm32f4xx_hal.h>

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义----------------  
//使用NOR/SRAM的 Bank1.Sector4,地址位HADDR[27,26]=11   A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			   
#define FSMC_LCD_CMD				0x6C000000U	    //FSMC_Bank1_NORSRAM4用于LCD命令操作的地址
#define FSMC_LCD_DATA				0x6C000080U     //FSMC_Bank1_NORSRAM4用于LCD数据操作的地址

#define WRITE_REG(CMD, DATA)		WRITE_CMD(CMD); WRITE_DATA(DATA)

#define WRITE_CMD(X)				*(__IO uint16_t *)FSMC_LCD_CMD  = X 
#define WRITE_DATA(X)				*(__IO uint16_t *)FSMC_LCD_DATA = X
#define READ_DATA()					*(__IO uint16_t *)FSMC_LCD_DATA
//////////////////////////////////////////////////////////////////////////////////

#define GBK_HZ1616_BASEADDR		0U
#define GBK_HZ2424_BASEADDR		266240U

//////////////////////////////////////////////////////////////////////////////////

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0xF81F
#define GRED 			 0xFFE0
#define GBLUE			 0x07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0xBC40 //棕色
#define BRRED 			 0xFC07 //棕红色
#define GRAY  			 0x7BEF //灰色
#define DARKGRAY  		 0x2124
//GUI颜色

#define DARKBLUE      	 0x01CF	//深蓝色
#define LIGHTBLUE      	 0x7D7C	//浅蓝色  
#define GRAYBLUE       	 0x5458 //灰蓝色
//以上三色为PANEL的颜色 

#define LIGHTGREEN     	 0x841F //浅绿色
#define LIGHTGRAY        0xEF5B //浅灰色(PANNEL)
#define LGRAY 			 0xC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0xA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0x2B12 //浅棕蓝色(选择条目的反色)


void LCD_Init(_Bool IsVerticalSreen);
void LCD_Clear(uint16_t Color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, uint16_t Color);
void LCD_FillRect(uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height, uint16_t Color);
void LCD_DrawPicture_Stream(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, uint16_t *pBuffer);
void LCD_DrawPicture_SD(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, const uint8_t* fileName);
void LCD_ShowNumber(uint16_t x, uint16_t y, uint8_t fontSize, uint32_t num, uint16_t Color);
void LCD_ShowBigNumber(uint16_t x, uint16_t y, uint8_t num, uint16_t Color);
void LCD_ShowString(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t *str, uint16_t Color);
static void LCD_ShowChar_ASCII(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t ch, uint16_t Color);
static void LCD_ShowChar_GBK(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t* ptr, uint16_t Color);

uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);
static inline void LCD_SetWindow(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t Color);

static void opt_delay(uint8_t);
extern void Delay_ms(uint16_t);
extern void Delay_us(uint32_t);

