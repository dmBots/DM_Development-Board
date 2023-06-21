#ifndef __ST7789V_H
#define __ST7789V_H
#include <stdint.h>
#include "LCD_1in69.h"
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint8_t	wramcmd;		//开始写gram指令
	uint8_t  setxcmd;		//设置x坐标指令
	uint8_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////	 
//ST7789寄存器定义
#define  ST7789_CMD_RDDID 0x04  //读取ID
#define  ST7789_CMD_SLPIN 0x10
#define  ST7789_CMD_SLPOUT 0x11
#define  ST7789_CMD_DISPOFF 0x28
#define  ST7789_CMD_DISPON 0x29
#define  ST7789_CMD_CASETF 0x2A
#define  ST7789_CMD_RASETF 0x2B
#define  ST7789_CMD_RAMWR 0x2C  //开始写内存数据
#define  ST7789_CMD_PTLON 0x12  //部分显示模式
#define  ST7789_CMD_NORON 0x13  //普通模式
#define  ST7789_CMD_INVOFF 0x20 //关闭反显示
#define  ST7789_CMD_INVON 0x21  //打开反显示
#define  ST7789_CMD_PTLAR 0x30  //部分显示区域 由上下两个水平坐标
	 
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

extern uint8_t DFT_SCAN_DIR;


//PC0~15,作为数据口
#define DATAOUT(x) GPIOC->ODR=x; //数据输出
#define DATAIN     GPIOC->IDR;   //数据输入

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init();

void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

void Fade_In();
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
void OLED_Printf(uint8_t x, uint8_t y,uint8_t size, const char* fmt, ...);
void OLED_DrawString(uint8_t x, uint8_t y,uint8_t size, uint8_t *buff);
void OLED_Clear(void);


void OLED_ShowString(uint8_t mode,uint8_t x,uint8_t y,uint8_t font, const char *p);
void UPDATE_GRAM(void);
void GRAM_Clear(void);
void OLED_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);

#endif
