#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"
#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif



//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port,LCD_SCLK_Pin,RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port,LCD_SCLK_Pin,SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port,LCD_MOSI_Pin,RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port,LCD_MOSI_Pin,SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RES_GPIO_Port,LCD_RES_Pin,SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,SET)

//#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOD,GPIO_Pin_1,RESET)//BLK
//#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOD,GPIO_Pin_1,SET)

//#define ZK_MISO        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//MISO  读取字库数据引脚

#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, RESET)//CS1
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, SET)

//#define ZK_CS_Clr()    HAL_GPIO_WritePin(GPIOE,GPIO_Pin_12,RESET)//CS2 字库片选
//#define ZK_CS_Set()    HAL_GPIO_WritePin(GPIOE,GPIO_Pin_12,SET)		




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




