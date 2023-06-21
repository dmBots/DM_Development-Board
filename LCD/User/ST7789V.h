#ifndef __ST7789V_H
#define __ST7789V_H
#include <stdint.h>
#include "LCD_1in69.h"
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
//LCD��Ҫ������
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������	
	uint8_t	wramcmd;		//��ʼдgramָ��
	uint8_t  setxcmd;		//����x����ָ��
	uint8_t  setycmd;		//����y����ָ��	 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//ST7789�Ĵ�������
#define  ST7789_CMD_RDDID 0x04  //��ȡID
#define  ST7789_CMD_SLPIN 0x10
#define  ST7789_CMD_SLPOUT 0x11
#define  ST7789_CMD_DISPOFF 0x28
#define  ST7789_CMD_DISPON 0x29
#define  ST7789_CMD_CASETF 0x2A
#define  ST7789_CMD_RASETF 0x2B
#define  ST7789_CMD_RAMWR 0x2C  //��ʼд�ڴ�����
#define  ST7789_CMD_PTLON 0x12  //������ʾģʽ
#define  ST7789_CMD_NORON 0x13  //��ͨģʽ
#define  ST7789_CMD_INVOFF 0x20 //�رշ���ʾ
#define  ST7789_CMD_INVON 0x21  //�򿪷���ʾ
#define  ST7789_CMD_PTLAR 0x30  //������ʾ���� ����������ˮƽ����
	 
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

extern uint8_t DFT_SCAN_DIR;


//PC0~15,��Ϊ���ݿ�
#define DATAOUT(x) GPIOC->ODR=x; //�������
#define DATAIN     GPIOC->IDR;   //��������

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

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
