#include "ST7789V.h"
#include "main.h"
#include "math.h"
#include <stdarg.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "oled_font.h"
#include <string.h>
#include "LCD_1in69.h"

extern SPI_HandleTypeDef hspi1;
uint8_t DFT_SCAN_DIR;
				 
//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t POINT_COLOR=0xFFFF;	//������ɫ
uint16_t BACK_COLOR=0x0000;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

/**************************************************************************************************************
 * ��������Write_Data()
 * ����  ��uint8_t RGB_data  8bit��ɫ����
 * ���  ��void
 * ����  ����LCD��RAMд����
 * ����  ���ⲿ����        
 *****************************************************************************************************************/
void Write_Data(uint8_t data)
{										    	   
    LCD_1IN69_DC_1;
    uint8_t temp = data;
    HAL_SPI_Transmit(&hspi1, &temp, 1, 100);
}


//д�Ĵ�������
//regval:�Ĵ���ֵ
void Write_Command(uint8_t regva)
{ 
	LCD_1IN69_DC_0;
    uint8_t temp = regva;
    HAL_SPI_Transmit(&hspi1, &temp, 1, 100);
}


//��LCD����
//����ֵ:������ֵ
static uint8_t ST7789_RD_DATA(void)
{
    uint8_t  data = 0;   
    //data = *(__IO uint8_t *) LCD_RAM;
    
    return data;
}
/*
*********************************************************************************************
*    �� �� ��: ST7789_ReadID
*    ����˵��: ��ȡLCD����оƬID
*    ��    ��:  ��
*    �� �� ֵ: ��
*********************************************************************************************
*/
uint16_t ST7789_ReadID(void)
{
    uint16_t id = 0;
    uint8_t  temp = 0;
    Write_Command(ST7789_CMD_RDDID);
    
    temp = ST7789_RD_DATA();  //The 1st parameter is dummy data
    temp = ST7789_RD_DATA();  //ID1  0x85
    
    temp = ST7789_RD_DATA();  //ID2  0x85
    
    id = temp<<8;

    
    temp = ST7789_RD_DATA();  //ID3  0x52
    
    id |= temp;

    return id ;
}

//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	Write_Command(LCD_Reg);
    Write_Data(LCD_RegValue>>8);
	Write_Data(LCD_RegValue&0XFF);
}	      
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	Write_Command(lcddev.wramcmd);
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	Write_Data(RGB_Code>>8);
    Write_Data(RGB_Code&0XFF);
}

//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(uint8_t i)
{
	while(i--);
}
  
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
	Write_Command(lcddev.setxcmd); 
	Write_Data(Xpos>>8); 
	Write_Data(Xpos&0XFF);	
  	
	Write_Command(lcddev.setycmd); 
	Write_Data(Ypos>>8); 
	Write_Data(Ypos&0XFF);
} 		 
//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint8_t dirreg=0;
	uint16_t temp;  
	switch(dir)//����ת��
	{
		case 0:dir=6;break;
		case 1:dir=7;break;
		case 2:dir=4;break;
		case 3:dir=5;break;
		case 4:dir=1;break;
		case 5:dir=0;break;
		case 6:dir=3;break;
		case 7:dir=2;break;	     
	}
	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ���
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://���ϵ���,������
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	dirreg=0X36; 
    regval|=0x00;	
	LCD_WriteReg(dirreg,regval);
			
	Write_Command(lcddev.setxcmd); 
	Write_Data(0);
    Write_Data(0);
	Write_Data((lcddev.width-1)>>8);
    Write_Data((lcddev.width-1)&0XFF);
    
	Write_Command(lcddev.setycmd); 
	Write_Data(0);
    Write_Data(0);
	Write_Data((lcddev.height-1)>>8);
    Write_Data((lcddev.height-1)&0XFF);  
		
  	
}      
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WriteRAM(POINT_COLOR); 
}

//����LCD��ʾ����6804��֧�ֺ�����ʾ��
//dir:0,������1,����
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;

		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
    DFT_SCAN_DIR=U2D_R2L;		

	}else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;

		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
    DFT_SCAN_DIR=L2R_U2D;		
		
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}


//��������
//color:Ҫ���������ɫ
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
    
	totalpoint*=lcddev.height; 	//�õ��ܵ���
    
    LCD_SetCursor(0x00,0x0000);	//���ù��λ��
    
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD_WriteRAM(color);	   
	}
} 

//��������
//color:Ҫ���������ɫ
void LCD_Clear1(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=320;
    
	totalpoint*=240; 	//�õ��ܵ���
    
    Write_Command(0X2A); 
	Write_Data(0x00); 
	Write_Data(0x00);	
  	
	Write_Command(0X2B); 
	Write_Data(0x00); 
	Write_Data(0x00);	//���ù��λ��
    
	Write_Command(0X2C);     //��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
        Write_Data(color>>8);
        Write_Data(color&0XFF);	   
	}
} 
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_WriteRAM(color);	//���ù��λ�� 	    
	}
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 		//�õ����Ŀ��
	height=ey-sy+1;		//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD_WriteRAM(color[i*height+j]);//д������ 
	}	  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint16_t colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//���ô���
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{   
	width=sx+width-1;
	height=sy+height-1;

	Write_Command(lcddev.setxcmd);
	Write_Data(sx>>8);  
	Write_Data(sx&0XFF);	  
	Write_Data(width>>8);   
	Write_Data(width&0XFF);   
	Write_Command(lcddev.setycmd);
	Write_Data(sy>>8);   
	Write_Data(sy&0XFF);  
	Write_Data(height>>8);   
	Write_Data(height&0XFF);  
}




//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LCD_Init()
{
    unsigned char i;
    uint16_t ID =0;
    
    LCD_1IN69_RST_1; 
	HAL_Delay(10);	
    LCD_1IN69_RST_0;  
	HAL_Delay(100);
	LCD_1IN69_RST_1; 
	HAL_Delay(10);
    
    //ID = ST7789_ReadID(); //0x8552
    


    Write_Command(0x11);              
    HAL_Delay(100); 

    Write_Command(0x36);			
    Write_Data(0x00);

    Write_Command(0x3A);			
    Write_Data(0x05);



    Write_Command(0xB2);			
    Write_Data(0x0C);
    Write_Data(0x0C); 
    Write_Data(0x00); 
    Write_Data(0x33); 
    Write_Data(0x33); 			

    Write_Command(0xB7);			
    Write_Data(0x35);

    Write_Command(0xBB);			
    Write_Data(0x19); //32 Vcom=1.35V
                                                        
    Write_Command(0xC2);			
    Write_Data(0x01);

    Write_Command(0xC3);			
    Write_Data(0x12); //GVDD=4.8V 
                                                          
    Write_Command(0xC4);			
    Write_Data(0x20); //VDV, 0x20:0v

    Write_Command(0xC5);			
    Write_Data(0x1A);//VCOM Offset Set

    Write_Command(0xC6);			
    Write_Data(0x0F); //0x0F:60Hz        	

    Write_Command(0xD0);			
    Write_Data(0xA7);//A4
    Write_Data(0xA1); 											  												  																								
            
      
    Write_Command( 0xE0);     
    Write_Data(0xD0);
    Write_Data(0x04);
    Write_Data(0x0D);
    Write_Data(0x11);
    Write_Data(0x13);
    Write_Data(0x2B);
    Write_Data(0x3F);
    Write_Data(0x54);
    Write_Data(0x4C);
    Write_Data(0x18);
    Write_Data(0x0D);
    Write_Data(0x0B);
    Write_Data(0x1F);
    Write_Data(0x23);  

    Write_Command( 0xE1);     
    Write_Data(0xD0);
    Write_Data(0x04);
    Write_Data(0x0C);
    Write_Data(0x11);
    Write_Data(0x13);
    Write_Data(0x2C);
    Write_Data(0x3F);
    Write_Data(0x44);
    Write_Data(0x51);
    Write_Data(0x2F);
    Write_Data(0x1F);
    Write_Data(0x1F);
    Write_Data(0x20);
    Write_Data(0x23);
     


    
    
    LCD_Display_Dir(0);//������ʾģʽ dir:0,������1,����

    LCD_Clear(WHITE);
    Write_Command(0x21);  //��ת��ʾ

    Write_Command(0x29);  //Display On
    

}
























