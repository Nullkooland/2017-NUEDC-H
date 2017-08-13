#include "lcd.h"
#include "ASCII.h" 
#include "BigNumber.h"
#include "FatFS/ff.h"

/* ILI9341 TFT LCD驱动程序 */
//////////////////////////////////////////////////////////////////////////////////	 

extern DMA_HandleTypeDef hdma_m2m;

static uint16_t SCREEN_WIDTH = 800;		//歪，默认是横屏哈，你娃听到没得！
static uint16_t SCREEN_HEIGHT = 480;


// SD FatFS文件操作相关变量
static uint8_t fileBuffer[512];
static FIL file;
static FRESULT f_res;
static UINT fnum;


uint16_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
	uint16_t r = 0, g = 0, b = 0;

	WRITE_REG(0x2A00, x >> 8);
	WRITE_REG(0x2A01, x & 0xFF);
	WRITE_REG(0x2B00, y >> 8);
	WRITE_REG(0x2B01, y & 0xFF);
	WRITE_CMD(0x2E00);

	READ_DATA();		//Dummy Read	   
	r = READ_DATA();	//实际坐标颜色
	b = READ_DATA();
	g = r & 0xFF;
	g <<= 8;
	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
}

static inline void LCD_SetWindow(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height)
{
	Width = x + Width - 1;
	Height = y + Height - 1;

	WRITE_REG(0x2A00, x >> 8);
	WRITE_REG(0x2A01, x & 0xFF);
	WRITE_REG(0x2A02, Width >> 8);
	WRITE_REG(0x2A03, Width & 0xFF);

	WRITE_REG(0x2B00, y >> 8);
	WRITE_REG(0x2B01, y & 0xFF);
	WRITE_REG(0x2B02, Height >> 8);
	WRITE_REG(0x2B03, Height & 0xFF);
}

void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t Color)
{
	WRITE_REG(0x2A00, x >> 8);
	WRITE_REG(0x2A01, x & 0xFF);
	WRITE_REG(0x2B00, y >> 8);
	WRITE_REG(0x2B01, y & 0xFF);
	WRITE_CMD(0x2C00);
	WRITE_DATA(Color);
}

void LCD_Init(_Bool IsVerticalScreen)
{
	Delay_ms(25);
	WRITE_REG(0x0000, 0x0001);
	Delay_ms(50); 

	FSMC_Bank1E->BWTR[6] &= ~(0xF << 0);	//地址建立时间（ADDSET）清零 	 
	FSMC_Bank1E->BWTR[6] &= ~(0xF << 8);	//数据保存时间清零
	FSMC_Bank1E->BWTR[6] |= 3 << 0;			//地址建立时间（ADDSET）为3个HCLK =18ns  	 
	FSMC_Bank1E->BWTR[6] |= 2 << 8; 		//数据保存时间为6ns*3个HCLK=18ns

	WRITE_REG(0xF000, 0x55);
	WRITE_REG(0xF001, 0xAA);
	WRITE_REG(0xF002, 0x52);
	WRITE_REG(0xF003, 0x08);
	WRITE_REG(0xF004, 0x01);

	//AVDD Set AVDD 5.2V
	WRITE_REG(0xB000, 0x0D);
	WRITE_REG(0xB001, 0x0D);
	WRITE_REG(0xB002, 0x0D);

	//AVDD ratio
	WRITE_REG(0xB600, 0x34);
	WRITE_REG(0xB601, 0x34);
	WRITE_REG(0xB602, 0x34);

	//AVEE -5.2V
	WRITE_REG(0xB100, 0x0D);
	WRITE_REG(0xB101, 0x0D);
	WRITE_REG(0xB102, 0x0D);

	//AVEE ratio
	WRITE_REG(0xB700, 0x34);
	WRITE_REG(0xB701, 0x34);
	WRITE_REG(0xB702, 0x34);

	//VCL -2.5V
	WRITE_REG(0xB200, 0x00);
	WRITE_REG(0xB201, 0x00);
	WRITE_REG(0xB202, 0x00);

	//VCL ratio
	WRITE_REG(0xB800, 0x24);
	WRITE_REG(0xB801, 0x24);
	WRITE_REG(0xB802, 0x24);

	//VGH 15V (Free pump)
	WRITE_REG(0xBF00, 0x01);
	WRITE_REG(0xB300, 0x0F);
	WRITE_REG(0xB301, 0x0F);
	WRITE_REG(0xB302, 0x0F);

	//VGH ratio
	WRITE_REG(0xB900, 0x34);
	WRITE_REG(0xB901, 0x34);
	WRITE_REG(0xB902, 0x34);

	//VGL_REG -10V
	WRITE_REG(0xB500, 0x08);
	WRITE_REG(0xB501, 0x08);
	WRITE_REG(0xB502, 0x08);
	WRITE_REG(0xC200, 0x03);

	//VGLX ratio
	WRITE_REG(0xBA00, 0x24);
	WRITE_REG(0xBA01, 0x24);
	WRITE_REG(0xBA02, 0x24);

	//VGMP/VGSP 4.5V/0V
	WRITE_REG(0xBC00, 0x00);
	WRITE_REG(0xBC01, 0x78);
	WRITE_REG(0xBC02, 0x00);

	//VGMN/VGSN -4.5V/0V
	WRITE_REG(0xBD00, 0x00);
	WRITE_REG(0xBD01, 0x78);
	WRITE_REG(0xBD02, 0x00);

	//VCOM
	WRITE_REG(0xBE00, 0x00);
	WRITE_REG(0xBE01, 0x64);

	//Gamma Setting
	WRITE_REG(0xD100, 0x00);
	WRITE_REG(0xD101, 0x33);
	WRITE_REG(0xD102, 0x00);
	WRITE_REG(0xD103, 0x34);
	WRITE_REG(0xD104, 0x00);
	WRITE_REG(0xD105, 0x3A);
	WRITE_REG(0xD106, 0x00);
	WRITE_REG(0xD107, 0x4A);
	WRITE_REG(0xD108, 0x00);
	WRITE_REG(0xD109, 0x5C);
	WRITE_REG(0xD10A, 0x00);
	WRITE_REG(0xD10B, 0x81);
	WRITE_REG(0xD10C, 0x00);
	WRITE_REG(0xD10D, 0xA6);
	WRITE_REG(0xD10E, 0x00);
	WRITE_REG(0xD10F, 0xE5);
	WRITE_REG(0xD110, 0x01);
	WRITE_REG(0xD111, 0x13);
	WRITE_REG(0xD112, 0x01);
	WRITE_REG(0xD113, 0x54);
	WRITE_REG(0xD114, 0x01);
	WRITE_REG(0xD115, 0x82);
	WRITE_REG(0xD116, 0x01);
	WRITE_REG(0xD117, 0xCA);
	WRITE_REG(0xD118, 0x02);
	WRITE_REG(0xD119, 0x00);
	WRITE_REG(0xD11A, 0x02);
	WRITE_REG(0xD11B, 0x01);
	WRITE_REG(0xD11C, 0x02);
	WRITE_REG(0xD11D, 0x34);
	WRITE_REG(0xD11E, 0x02);
	WRITE_REG(0xD11F, 0x67);
	WRITE_REG(0xD120, 0x02);
	WRITE_REG(0xD121, 0x84);
	WRITE_REG(0xD122, 0x02);
	WRITE_REG(0xD123, 0xA4);
	WRITE_REG(0xD124, 0x02);
	WRITE_REG(0xD125, 0xB7);
	WRITE_REG(0xD126, 0x02);
	WRITE_REG(0xD127, 0xCF);
	WRITE_REG(0xD128, 0x02);
	WRITE_REG(0xD129, 0xDE);
	WRITE_REG(0xD12A, 0x02);
	WRITE_REG(0xD12B, 0xF2);
	WRITE_REG(0xD12C, 0x02);
	WRITE_REG(0xD12D, 0xFE);
	WRITE_REG(0xD12E, 0x03);
	WRITE_REG(0xD12F, 0x10);
	WRITE_REG(0xD130, 0x03);
	WRITE_REG(0xD131, 0x33);
	WRITE_REG(0xD132, 0x03);
	WRITE_REG(0xD133, 0x6D);
	WRITE_REG(0xD200, 0x00);
	WRITE_REG(0xD201, 0x33);
	WRITE_REG(0xD202, 0x00);
	WRITE_REG(0xD203, 0x34);
	WRITE_REG(0xD204, 0x00);
	WRITE_REG(0xD205, 0x3A);
	WRITE_REG(0xD206, 0x00);
	WRITE_REG(0xD207, 0x4A);
	WRITE_REG(0xD208, 0x00);
	WRITE_REG(0xD209, 0x5C);
	WRITE_REG(0xD20A, 0x00);

	WRITE_REG(0xD20B, 0x81);
	WRITE_REG(0xD20C, 0x00);
	WRITE_REG(0xD20D, 0xA6);
	WRITE_REG(0xD20E, 0x00);
	WRITE_REG(0xD20F, 0xE5);
	WRITE_REG(0xD210, 0x01);
	WRITE_REG(0xD211, 0x13);
	WRITE_REG(0xD212, 0x01);
	WRITE_REG(0xD213, 0x54);
	WRITE_REG(0xD214, 0x01);
	WRITE_REG(0xD215, 0x82);
	WRITE_REG(0xD216, 0x01);
	WRITE_REG(0xD217, 0xCA);
	WRITE_REG(0xD218, 0x02);
	WRITE_REG(0xD219, 0x00);
	WRITE_REG(0xD21A, 0x02);
	WRITE_REG(0xD21B, 0x01);
	WRITE_REG(0xD21C, 0x02);
	WRITE_REG(0xD21D, 0x34);
	WRITE_REG(0xD21E, 0x02);
	WRITE_REG(0xD21F, 0x67);
	WRITE_REG(0xD220, 0x02);
	WRITE_REG(0xD221, 0x84);
	WRITE_REG(0xD222, 0x02);
	WRITE_REG(0xD223, 0xA4);
	WRITE_REG(0xD224, 0x02);
	WRITE_REG(0xD225, 0xB7);
	WRITE_REG(0xD226, 0x02);
	WRITE_REG(0xD227, 0xCF);
	WRITE_REG(0xD228, 0x02);
	WRITE_REG(0xD229, 0xDE);
	WRITE_REG(0xD22A, 0x02);
	WRITE_REG(0xD22B, 0xF2);
	WRITE_REG(0xD22C, 0x02);
	WRITE_REG(0xD22D, 0xFE);
	WRITE_REG(0xD22E, 0x03);
	WRITE_REG(0xD22F, 0x10);
	WRITE_REG(0xD230, 0x03);
	WRITE_REG(0xD231, 0x33);
	WRITE_REG(0xD232, 0x03);
	WRITE_REG(0xD233, 0x6D);
	WRITE_REG(0xD300, 0x00);
	WRITE_REG(0xD301, 0x33);
	WRITE_REG(0xD302, 0x00);
	WRITE_REG(0xD303, 0x34);
	WRITE_REG(0xD304, 0x00);
	WRITE_REG(0xD305, 0x3A);
	WRITE_REG(0xD306, 0x00);
	WRITE_REG(0xD307, 0x4A);
	WRITE_REG(0xD308, 0x00);
	WRITE_REG(0xD309, 0x5C);
	WRITE_REG(0xD30A, 0x00);

	WRITE_REG(0xD30B, 0x81);
	WRITE_REG(0xD30C, 0x00);
	WRITE_REG(0xD30D, 0xA6);
	WRITE_REG(0xD30E, 0x00);
	WRITE_REG(0xD30F, 0xE5);
	WRITE_REG(0xD310, 0x01);
	WRITE_REG(0xD311, 0x13);
	WRITE_REG(0xD312, 0x01);
	WRITE_REG(0xD313, 0x54);
	WRITE_REG(0xD314, 0x01);
	WRITE_REG(0xD315, 0x82);
	WRITE_REG(0xD316, 0x01);
	WRITE_REG(0xD317, 0xCA);
	WRITE_REG(0xD318, 0x02);
	WRITE_REG(0xD319, 0x00);
	WRITE_REG(0xD31A, 0x02);
	WRITE_REG(0xD31B, 0x01);
	WRITE_REG(0xD31C, 0x02);
	WRITE_REG(0xD31D, 0x34);
	WRITE_REG(0xD31E, 0x02);
	WRITE_REG(0xD31F, 0x67);
	WRITE_REG(0xD320, 0x02);
	WRITE_REG(0xD321, 0x84);
	WRITE_REG(0xD322, 0x02);
	WRITE_REG(0xD323, 0xA4);
	WRITE_REG(0xD324, 0x02);
	WRITE_REG(0xD325, 0xB7);
	WRITE_REG(0xD326, 0x02);
	WRITE_REG(0xD327, 0xCF);
	WRITE_REG(0xD328, 0x02);
	WRITE_REG(0xD329, 0xDE);
	WRITE_REG(0xD32A, 0x02);
	WRITE_REG(0xD32B, 0xF2);
	WRITE_REG(0xD32C, 0x02);
	WRITE_REG(0xD32D, 0xFE);
	WRITE_REG(0xD32E, 0x03);
	WRITE_REG(0xD32F, 0x10);
	WRITE_REG(0xD330, 0x03);
	WRITE_REG(0xD331, 0x33);
	WRITE_REG(0xD332, 0x03);
	WRITE_REG(0xD333, 0x6D);
	WRITE_REG(0xD400, 0x00);
	WRITE_REG(0xD401, 0x33);
	WRITE_REG(0xD402, 0x00);
	WRITE_REG(0xD403, 0x34);
	WRITE_REG(0xD404, 0x00);
	WRITE_REG(0xD405, 0x3A);
	WRITE_REG(0xD406, 0x00);
	WRITE_REG(0xD407, 0x4A);
	WRITE_REG(0xD408, 0x00);
	WRITE_REG(0xD409, 0x5C);
	WRITE_REG(0xD40A, 0x00);
	WRITE_REG(0xD40B, 0x81);

	WRITE_REG(0xD40C, 0x00);
	WRITE_REG(0xD40D, 0xA6);
	WRITE_REG(0xD40E, 0x00);
	WRITE_REG(0xD40F, 0xE5);
	WRITE_REG(0xD410, 0x01);
	WRITE_REG(0xD411, 0x13);
	WRITE_REG(0xD412, 0x01);
	WRITE_REG(0xD413, 0x54);
	WRITE_REG(0xD414, 0x01);
	WRITE_REG(0xD415, 0x82);
	WRITE_REG(0xD416, 0x01);
	WRITE_REG(0xD417, 0xCA);
	WRITE_REG(0xD418, 0x02);
	WRITE_REG(0xD419, 0x00);
	WRITE_REG(0xD41A, 0x02);
	WRITE_REG(0xD41B, 0x01);
	WRITE_REG(0xD41C, 0x02);
	WRITE_REG(0xD41D, 0x34);
	WRITE_REG(0xD41E, 0x02);
	WRITE_REG(0xD41F, 0x67);
	WRITE_REG(0xD420, 0x02);
	WRITE_REG(0xD421, 0x84);
	WRITE_REG(0xD422, 0x02);
	WRITE_REG(0xD423, 0xA4);
	WRITE_REG(0xD424, 0x02);
	WRITE_REG(0xD425, 0xB7);
	WRITE_REG(0xD426, 0x02);
	WRITE_REG(0xD427, 0xCF);
	WRITE_REG(0xD428, 0x02);
	WRITE_REG(0xD429, 0xDE);
	WRITE_REG(0xD42A, 0x02);
	WRITE_REG(0xD42B, 0xF2);
	WRITE_REG(0xD42C, 0x02);
	WRITE_REG(0xD42D, 0xFE);
	WRITE_REG(0xD42E, 0x03);
	WRITE_REG(0xD42F, 0x10);
	WRITE_REG(0xD430, 0x03);
	WRITE_REG(0xD431, 0x33);
	WRITE_REG(0xD432, 0x03);
	WRITE_REG(0xD433, 0x6D);
	WRITE_REG(0xD500, 0x00);
	WRITE_REG(0xD501, 0x33);
	WRITE_REG(0xD502, 0x00);
	WRITE_REG(0xD503, 0x34);
	WRITE_REG(0xD504, 0x00);
	WRITE_REG(0xD505, 0x3A);
	WRITE_REG(0xD506, 0x00);
	WRITE_REG(0xD507, 0x4A);
	WRITE_REG(0xD508, 0x00);
	WRITE_REG(0xD509, 0x5C);
	WRITE_REG(0xD50A, 0x00);
	WRITE_REG(0xD50B, 0x81);

	WRITE_REG(0xD50C, 0x00);
	WRITE_REG(0xD50D, 0xA6);
	WRITE_REG(0xD50E, 0x00);
	WRITE_REG(0xD50F, 0xE5);
	WRITE_REG(0xD510, 0x01);
	WRITE_REG(0xD511, 0x13);
	WRITE_REG(0xD512, 0x01);
	WRITE_REG(0xD513, 0x54);
	WRITE_REG(0xD514, 0x01);
	WRITE_REG(0xD515, 0x82);
	WRITE_REG(0xD516, 0x01);
	WRITE_REG(0xD517, 0xCA);
	WRITE_REG(0xD518, 0x02);
	WRITE_REG(0xD519, 0x00);
	WRITE_REG(0xD51A, 0x02);
	WRITE_REG(0xD51B, 0x01);
	WRITE_REG(0xD51C, 0x02);
	WRITE_REG(0xD51D, 0x34);
	WRITE_REG(0xD51E, 0x02);
	WRITE_REG(0xD51F, 0x67);
	WRITE_REG(0xD520, 0x02);
	WRITE_REG(0xD521, 0x84);
	WRITE_REG(0xD522, 0x02);
	WRITE_REG(0xD523, 0xA4);
	WRITE_REG(0xD524, 0x02);
	WRITE_REG(0xD525, 0xB7);
	WRITE_REG(0xD526, 0x02);
	WRITE_REG(0xD527, 0xCF);
	WRITE_REG(0xD528, 0x02);
	WRITE_REG(0xD529, 0xDE);
	WRITE_REG(0xD52A, 0x02);
	WRITE_REG(0xD52B, 0xF2);
	WRITE_REG(0xD52C, 0x02);
	WRITE_REG(0xD52D, 0xFE);
	WRITE_REG(0xD52E, 0x03);
	WRITE_REG(0xD52F, 0x10);
	WRITE_REG(0xD530, 0x03);
	WRITE_REG(0xD531, 0x33);
	WRITE_REG(0xD532, 0x03);
	WRITE_REG(0xD533, 0x6D);
	WRITE_REG(0xD600, 0x00);
	WRITE_REG(0xD601, 0x33);
	WRITE_REG(0xD602, 0x00);
	WRITE_REG(0xD603, 0x34);
	WRITE_REG(0xD604, 0x00);
	WRITE_REG(0xD605, 0x3A);
	WRITE_REG(0xD606, 0x00);
	WRITE_REG(0xD607, 0x4A);
	WRITE_REG(0xD608, 0x00);
	WRITE_REG(0xD609, 0x5C);
	WRITE_REG(0xD60A, 0x00);
	WRITE_REG(0xD60B, 0x81);

	WRITE_REG(0xD60C, 0x00);
	WRITE_REG(0xD60D, 0xA6);
	WRITE_REG(0xD60E, 0x00);
	WRITE_REG(0xD60F, 0xE5);
	WRITE_REG(0xD610, 0x01);
	WRITE_REG(0xD611, 0x13);
	WRITE_REG(0xD612, 0x01);
	WRITE_REG(0xD613, 0x54);
	WRITE_REG(0xD614, 0x01);
	WRITE_REG(0xD615, 0x82);
	WRITE_REG(0xD616, 0x01);
	WRITE_REG(0xD617, 0xCA);
	WRITE_REG(0xD618, 0x02);
	WRITE_REG(0xD619, 0x00);
	WRITE_REG(0xD61A, 0x02);
	WRITE_REG(0xD61B, 0x01);
	WRITE_REG(0xD61C, 0x02);
	WRITE_REG(0xD61D, 0x34);
	WRITE_REG(0xD61E, 0x02);
	WRITE_REG(0xD61F, 0x67);
	WRITE_REG(0xD620, 0x02);
	WRITE_REG(0xD621, 0x84);
	WRITE_REG(0xD622, 0x02);
	WRITE_REG(0xD623, 0xA4);
	WRITE_REG(0xD624, 0x02);
	WRITE_REG(0xD625, 0xB7);
	WRITE_REG(0xD626, 0x02);
	WRITE_REG(0xD627, 0xCF);
	WRITE_REG(0xD628, 0x02);
	WRITE_REG(0xD629, 0xDE);
	WRITE_REG(0xD62A, 0x02);
	WRITE_REG(0xD62B, 0xF2);
	WRITE_REG(0xD62C, 0x02);
	WRITE_REG(0xD62D, 0xFE);
	WRITE_REG(0xD62E, 0x03);
	WRITE_REG(0xD62F, 0x10);
	WRITE_REG(0xD630, 0x03);
	WRITE_REG(0xD631, 0x33);
	WRITE_REG(0xD632, 0x03);
	WRITE_REG(0xD633, 0x6D);

	//LV2 Page 0 enable
	WRITE_REG(0xF000, 0x55);
	WRITE_REG(0xF001, 0xAA);
	WRITE_REG(0xF002, 0x52);
	WRITE_REG(0xF003, 0x08);
	WRITE_REG(0xF004, 0x00);

	//Display control
	WRITE_REG(0xB100, 0xCC);
	WRITE_REG(0xB101, 0x00);

	//Source hold time
	WRITE_REG(0xB600, 0x05);
	//Gate EQ control
	WRITE_REG(0xB700, 0x70);
	WRITE_REG(0xB701, 0x70);

	//Source EQ control (Mode 2)
	WRITE_REG(0xB800, 0x01);
	WRITE_REG(0xB801, 0x03);
	WRITE_REG(0xB802, 0x03);
	WRITE_REG(0xB803, 0x03);

	//Inversion mode (2-dot)
	WRITE_REG(0xBC00, 0x02);
	WRITE_REG(0xBC01, 0x00);
	WRITE_REG(0xBC02, 0x00);

	//Timing control 4H w/ 4-delay
	WRITE_REG(0xC900, 0xD0);
	WRITE_REG(0xC901, 0x02);
	WRITE_REG(0xC902, 0x50);
	WRITE_REG(0xC903, 0x50);
	WRITE_REG(0xC904, 0x50);

	//16-Bit / Pixel
	WRITE_REG(0x3500, 0x00);
	WRITE_REG(0x3A00, 0x55);  

	if (IsVerticalScreen)
	{
		uint16_t TEMP = SCREEN_WIDTH;
		SCREEN_WIDTH = SCREEN_HEIGHT;
		SCREEN_HEIGHT = TEMP;
		WRITE_REG(0x3600, 0x03);
	}
	else
	{
		WRITE_REG(0x3600, 0x21);
	}

	LCD_Clear(BLACK);

	WRITE_CMD(0x1100);
	Delay_us(120);
	WRITE_CMD(0x2900); //开屏

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIOA->BSRR = GPIO_PIN_6; //点亮背光
}

void LCD_Clear(uint16_t Color)
{
	LCD_SetWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	WRITE_CMD(0x2C00);  	  
	for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		WRITE_DATA(Color);
	}
}

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, uint16_t Color)
{
	LCD_SetWindow(x, y, Width, Height);
	WRITE_CMD(0x2C00);
	for (uint32_t i = 0; i < Width * Height; i++)
	{
		WRITE_DATA(Color);
	}

}

void LCD_DrawPicture_Stream(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, uint16_t *pBuffer)
{
	LCD_SetWindow(x, y, Width, Height);
	WRITE_CMD(0x2C00);

	HAL_DMA_Start(&hdma_m2m, pBuffer, FSMC_LCD_DATA, Width * Height);
	HAL_DMA_PollForTransfer(&hdma_m2m, HAL_DMA_FULL_TRANSFER, 0xFF);
}

void LCD_DrawPicture_SD(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, const uint8_t* fileName)
{
	f_res = f_open(&file, fileName, FA_OPEN_EXISTING | FA_READ);
	
	if (f_res == FR_OK)
	{
		LCD_SetWindow(x, y, Width, Height);
		WRITE_CMD(0x2C00);

		do
		{
			f_read(&file, &fileBuffer, 512U, &fnum);
			uint16_t* imgBuffer = (uint16_t *)fileBuffer;

			for (uint16_t i = 0; i < fnum / 2; i++)
			{
				WRITE_DATA(imgBuffer[i]);
			}

		} while (fnum > 0);
	}

	f_close(&file);
	
	/*
	f_read(&file, (uint8_t*)0x68000000U, Width * Height * 2, &fnum);
	f_close(&file);

	LCD_SetWindow(x, y, Width, Height);
	WRITE_CMD(0x2C00);
	
	uint16_t * imgBuffer = (uint16_t*)0x68000000U;
	for (uint32_t i = 0; i < Width * Height; i++) WRITE_DATA(imgBuffer[i]);*/
}
 
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)incx = 1; //设置单步方向 
	else if (delta_x == 0)incx = 0;//垂直线 
	else { incx = -1; delta_x = -delta_x; }
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//水平线 
	else { incy = -1; delta_y = -delta_y; }
	if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴 
	else distance = delta_y;
	for (t = 0; t <= distance + 1; t++)//画线输出 
	{
		LCD_DrawPixel(uRow, uCol, Color);//画点 
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t Width, uint16_t Height, uint16_t Color)
{
	Width += x;
	Height += y;

	for (uint16_t i = x; i < Width; i++)
	{
		LCD_DrawPixel(i, y, Color);
		LCD_DrawPixel(i, Height, Color);
	}

	for (uint16_t i = y; i < Height; i++)
	{
		LCD_DrawPixel(x, i, Color);
		LCD_DrawPixel(Width, i, Color);
	}
}

void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t Radius,uint16_t Color)
{
	int a, b;
	int di;
	a = 0; b = Radius;
	di = 3 - (Radius << 1);             //判断下个点位置的标志
	while (a <= b)
	{
		LCD_DrawPixel(x0 + a, y0 - b ,Color);             //5
		LCD_DrawPixel(x0 + b, y0 - a, Color);             //0           
		LCD_DrawPixel(x0 + b, y0 + a, Color);             //4               
		LCD_DrawPixel(x0 + a, y0 + b, Color);             //6 
		LCD_DrawPixel(x0 - a, y0 + b, Color);             //1       
		LCD_DrawPixel(x0 - b, y0 + a, Color);
		LCD_DrawPixel(x0 - a, y0 - b, Color);             //2             
		LCD_DrawPixel(x0 - b, y0 - a, Color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if (di < 0)di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}

void LCD_ShowNumber(uint16_t x, uint16_t y, uint8_t fontSize, uint32_t num, uint16_t Color)
{
	uint8_t temp[12] = { 0 };
	uint8_t i = 10;

	do
	{
		temp[i--] = num % 10 + '0';
		num /= 10;
	} while (num);

	LCD_ShowString(x, y, fontSize, temp + i + 1, Color);
}

void LCD_ShowBigNumber(uint16_t x, uint16_t y,  uint8_t num, uint16_t Color)
{
	uint16_t x0 = x;

	for (uint16_t i = 0; i < 256; i++)
	{
		uint8_t temp = BigNumber_64x32[num][i];

		for (uint8_t j = 0; j < 8; j++)
		{
			if (temp & 1) LCD_DrawPixel(x, y, Color);

			temp >>= 1;
			x++;
			if (x >= SCREEN_WIDTH)	return;
			if ((x - x0) == 32)
			{
				x = x0;
				y++;
				if (y >= SCREEN_HEIGHT) return;
				break;
			}
		}
	}
		
}

void LCD_ShowString(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t *str, uint16_t Color)
{
	switch (fontSize)
	{
	case 16:
		f_res = f_open(&file, "0:GBK_16x16.fon", FA_OPEN_EXISTING | FA_READ);
		break;
	case 24:
		f_res = f_open(&file, "0:GBK_24x24.fon", FA_OPEN_EXISTING | FA_READ);
		break;
	case 32:
		f_res = f_open(&file, "0:GBK_32x32.fon", FA_OPEN_EXISTING | FA_READ);
		break;
	default: return;

	}

	while (*str)
	{
		if (*str < 0x80)
		{
			LCD_ShowChar_ASCII(x, y, fontSize, *(str++), Color);
			x += fontSize / 2;
		}
		else
		{
			LCD_ShowChar_GBK(x, y, fontSize, str++, Color);
			str++;
			x += fontSize;
		}
	}

	f_close(&file);
}

static void LCD_ShowChar_ASCII(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t ch, uint16_t Color)
{
	uint16_t x0 = x;
	uint8_t* buffer;
	uint8_t bufferSize = ((fontSize >> 3) + ((fontSize % 8) ? 1 : 0)) * (fontSize >> 1);		//得到字体一个字符对应点阵集所占的字节数

	ch = ch - ' ';
	switch (fontSize)
	{
		case 16: buffer = ASCII_16x8[ch]; break;
		case 32: buffer = ASCII_32x16[ch]; break;
		default: return;
	}

	for (uint8_t i = 0; i < bufferSize; i++)
	{
		uint8_t temp = buffer[i];

		for (uint8_t j = 0; j < 8; j++)
		{
			if (temp & 1) LCD_DrawPixel(x, y, Color);

			temp >>= 1;
			x++;
			if (x >= SCREEN_WIDTH)	return;
			if ((x - x0) == fontSize / 2)
			{
				x = x0;
				y++;
				if (y >= SCREEN_HEIGHT) return;
				break;
			}
		}
	}
}

static void LCD_ShowChar_GBK(uint16_t x, uint16_t y, uint8_t fontSize, uint8_t* ptr, uint16_t Color)
{
	uint8_t bufferSize = fontSize * fontSize >> 3;
	uint16_t x0 = x, temp;

	f_lseek(&file, ((ptr[0] - 0xA1) * 94 + (ptr[1] - 0xA1)) * bufferSize);
	f_read(&file, &fileBuffer, bufferSize, &fnum);

	for (uint8_t i = 0; i < bufferSize; i++)
	{
		temp = fileBuffer[i];

		for (uint8_t j = 0; j < 8; j++)
		{
			if (temp & 1) LCD_DrawPixel(x, y, Color);

			temp >>= 1;
			x++;
			if ((x - x0) == fontSize)
			{
				x = x0;
				y++;
				break;
			}
		}
	}
}
