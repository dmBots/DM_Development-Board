#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"
#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif



//-----------------LCD�˿ڶ���---------------- 

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

//#define ZK_MISO        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//MISO  ��ȡ�ֿ���������

#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, RESET)//CS1
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, SET)

//#define ZK_CS_Clr()    HAL_GPIO_WritePin(GPIOE,GPIO_Pin_12,RESET)//CS2 �ֿ�Ƭѡ
//#define ZK_CS_Set()    HAL_GPIO_WritePin(GPIOE,GPIO_Pin_12,SET)		




void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(uint8_t dat);//ģ��SPIʱ��
void LCD_WR_DATA8(uint8_t dat);//д��һ���ֽ�
void LCD_WR_DATA(uint16_t dat);//д�������ֽ�
void LCD_WR_REG(uint8_t dat);//д��һ��ָ��
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif




