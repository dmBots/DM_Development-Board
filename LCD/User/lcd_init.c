#include "lcd_init.h"
#include "main.h"

void LCD_GPIO_Init(void)
{

}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
    uint8_t i;
    LCD_CS_Clr();
    for(i=0;i<8;i++)
    {			  
        LCD_SCLK_Clr();
        if(dat&0x80)
        {
           LCD_MOSI_Set();
        }
        else
        {
           LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat<<=1;
    }	
    LCD_CS_Set();	
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+0x14);
		LCD_WR_DATA(y2+0x14);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+0x14);
		LCD_WR_DATA(y2+0x14);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+0x14);
		LCD_WR_DATA(x2+0x14);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+0x14);
		LCD_WR_DATA(x2+0x14);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//��ʼ��GPIO
	
	LCD_RES_Clr();//��λ
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	//LCD_BLK_Set();//�򿪱���
  HAL_Delay(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	HAL_Delay(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);			
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);			
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x33); 			

	LCD_WR_REG(0xB7);			
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);			
	LCD_WR_DATA8(0x32); //Vcom=1.35V
					
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			
	LCD_WR_DATA8(0x15); //GVDD=4.8V  ��ɫ���
				
	LCD_WR_REG(0xC4);			
	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	LCD_WR_REG(0xC6);			
	LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	LCD_WR_REG(0xD0);			
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1); 

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);
    //    LCD_GPIO_Init();//��ʼ��GPIO

//    LCD_RES_Clr();//��λ
//    HAL_Delay(100); 
//    LCD_RES_Set();
//    HAL_Delay(100);

//    //LCD_BLK_Set();//�򿪱���
//    HAL_Delay(100);
//	
//	//************* Start Initial Sequence **********//
//	LCD_WR_REG(0x11); //Sleep out 
//	HAL_Delay(120);              //Delay 120ms 
//	//************* Start Initial Sequence **********// 
//	LCD_WR_REG(0x36);
//	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
//	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
//	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
//	else LCD_WR_DATA8(0xA0);

//	LCD_WR_REG(0x3A);			
//	LCD_WR_DATA8(0x06);

//	LCD_WR_REG(0xB2);			
//	LCD_WR_DATA8(0x0B);
//	LCD_WR_DATA8(0x0B); 
//	LCD_WR_DATA8(0x00); 
//	LCD_WR_DATA8(0x33); 
//	LCD_WR_DATA8(0x33); 			

//	LCD_WR_REG(0xB7);			
//	LCD_WR_DATA8(0x35);

//	LCD_WR_REG(0xBB);			
//	LCD_WR_DATA8(0x23); //Vcom=1.35V
//					
//	LCD_WR_REG(0xC0);
//	LCD_WR_DATA8(0x2C);
//    
//    LCD_WR_REG(0xC2);
//	LCD_WR_DATA8(0x01);

//	LCD_WR_REG(0xC3);			
//	LCD_WR_DATA8(0x0D); //GVDD=4.8V  ��ɫ���
//				
//	LCD_WR_REG(0xC4);			
//	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

//	LCD_WR_REG(0xC6);			
//	LCD_WR_DATA8(0x13); //0x0F:60Hz        	

//	LCD_WR_REG(0xD0);			
//	LCD_WR_DATA8(0xA4);
//	LCD_WR_DATA8(0xA1); 
//    
//    LCD_WR_REG(0xD6);			
//	LCD_WR_DATA8(0xA1);

//	LCD_WR_REG(0xE0);
//	LCD_WR_DATA8(0xF0);   
//	LCD_WR_DATA8(0x02);   
//	LCD_WR_DATA8(0x05);   
//	LCD_WR_DATA8(0x07);   
//	LCD_WR_DATA8(0x06);   
//	LCD_WR_DATA8(0x03);   
//	LCD_WR_DATA8(0x26);   
//	LCD_WR_DATA8(0x33);   
//	LCD_WR_DATA8(0x3D);   
//	LCD_WR_DATA8(0x36);   
//	LCD_WR_DATA8(0x12);   
//	LCD_WR_DATA8(0x12);   
//	LCD_WR_DATA8(0x24);   
//	LCD_WR_DATA8(0x2B);   

//	LCD_WR_REG(0xE1);     
//	LCD_WR_DATA8(0xF0);   
//	LCD_WR_DATA8(0x04);   
//	LCD_WR_DATA8(0x07);   
//	LCD_WR_DATA8(0x09);   
//	LCD_WR_DATA8(0x08);   
//	LCD_WR_DATA8(0x25);   
//	LCD_WR_DATA8(0x25);   
//	LCD_WR_DATA8(0x33);   
//	LCD_WR_DATA8(0x3D);   
//	LCD_WR_DATA8(0x37);   
//	LCD_WR_DATA8(0x14);   
//	LCD_WR_DATA8(0x14);   
//	LCD_WR_DATA8(0x26);   
//	LCD_WR_DATA8(0x2C);
// 

//	LCD_WR_REG(0xE4);
//    LCD_WR_DATA8(0x25);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x00);
//    
//    LCD_WR_REG(0x21);
//    
//    LCD_WR_REG(0x29);
//    
//    LCD_WR_REG(0x2A);
//    LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0xEF);
//    
//    LCD_WR_REG(0x2B);
//    LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x14);   
//	LCD_WR_DATA8(0x01);   
//	LCD_WR_DATA8(0x2B);
//    
//    LCD_WR_REG(0x2C);
    
} 







