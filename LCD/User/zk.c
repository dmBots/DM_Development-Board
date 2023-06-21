#include "lcd_init.h"
#include "lcd.h"

u8 FontBuf[130];//�ֿ⻺��	
/******************************************************************************
      ����˵�������ֿ�д������
      ������ݣ�dat  Ҫд�������
      ����ֵ��  ��
******************************************************************************/
void ZK_command(u8 dat)
{
	u8 i; 
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
 }

/******************************************************************************
      ����˵�������ֿ��ȡ����
      ������ݣ���
      ����ֵ��  ret_data ��ȡ������
******************************************************************************/
u8 get_data_from_ROM(void)
{
	u8 i;
	u8 ret_data=0; //�������ݳ�ʼ��
	for(i=0;i<8;i++)
	{
		LCD_SCLK_Clr();  //�ֿ�ʱ������  
		ret_data<<=1;
		if(ZK_MISO)
		{
			ret_data++;
		}
		LCD_SCLK_Set(); //�ֿ�ʱ������
	}
	return ret_data;    //���ض�����һ���ֽ�
}
 
/******************************************************************************
      ����˵������ȡN������
      ������ݣ�AddrHigh  д��ַ���ֽ�
                AddrMid   д��ַ���ֽ�
                AddrLow   д��ַ���ֽ�
                *pBuff    ��ȡ������
                DataLen   ��ȡ���ݵĳ���
      ����ֵ��  ��
******************************************************************************/
void get_n_bytes_data_from_ROM(u8 AddrHigh,u8 AddrMid,u8 AddrLow,u8 *pBuff,u8 DataLen)
{
 	u8 i;
	ZK_CS_Clr(); //�ֿ�Ƭѡ
	ZK_command(0x03);//дָ��
	ZK_command(AddrHigh);//д��ַ���ֽ�
	ZK_command(AddrMid);//д��ַ���ֽ�
	ZK_command(AddrLow);//д��ַ���ֽ�
	for(i=0;i<DataLen;i++)
	{
	   *(pBuff+i) =get_data_from_ROM();//��һ���ֽ�����
	}

	ZK_CS_Set();//ȡ���ֿ�Ƭѡ
}

/******************************************************************************
      ����˵������ʾ����
      ������ݣ�x,y      д�������
                zk_num   1:12*12,  2:15*16,  3:24*24,  4:32*32
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_GB2312(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc)
{
  u8 i,k,n,d,m=0;
	switch(zk_num)
	{
		// n:�ַ���ռ�ֽ���  d���ּ��
		case 12 :  n=24;  d=12; break;   // 12*12
		case 16 :  n=32;  d=16; break;   // 15*16
		case 24 :  n=72;  d=24; break;   // 24*24
		case 32 :  n=128; d=32; break;   // 32*32
	}
	LCD_Address_Set(x,y,x+d-1,y+d-1);
	for(i=0;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(fc);
			}
			else
			{
			  LCD_WR_DATA(bc);
			}
			m++;
			if(m%zk_num==0)
			{
				m=0;
				break;
			}
		}
	}
}

 
/******************************************************************************
      ����˵������ʾ����
      ������ݣ�zk_num    1:12*12,  2:15*16,  3:24*24,  4:32*32
                x,y       ����
                text[]    Ҫ��ʾ�ĺ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_GB2312_String(u16 x,u16 y,u8 zk_num,u8 text[],u16 fc,u16 bc)
{
	u8 i= 0;
	u8 AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	u32 FontAddr=0; //�ֵ�ַ
	u32 BaseAdd=0; //�ֿ����ַ	
	u8 n,d;// ��ͬ�����ֿ�ļ������
	switch(zk_num)
	{
		// n:�ַ���ռ�ֽ���  d���ּ��
		case 12 :  BaseAdd=0x00;    n=24;  d=12; break;   // 12*12  
		case 16 :  BaseAdd=0x2C9D0; n=32;  d=16; break;   // 15*16  
		case 24 :  BaseAdd=0x68190; n=72;  d=24; break;   // 24*24  
		case 32 :  BaseAdd=0xEDF00; n=128; d=32; break;   // 32*32  
	}
	while((text[i]>0x00))
	{
		if(((text[i]>=0xA1)&&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
		{						
			//������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
			//Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ///
			FontAddr = (text[i]- 0xA1)*94; 
			FontAddr += (text[i+1]-0xA1);
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"
			Display_GB2312(x,y,zk_num,fc,bc);//��ʾһ�����ֵ�LCD��/ 
		}
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{
			//������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ//
			FontAddr = (text[i]- 0xB0)*94; 
			FontAddr += (text[i+1]-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"
			Display_GB2312(x,y,zk_num,fc,bc);//��ʾһ�����ֵ�LCD��/
		}
		x+=d; //��һ��������
		i+=2;  //�¸��ַ�
	}
} 
 
/******************************************************************************
      ����˵������ʾASCII��
      ������ݣ�x,y      д�������
                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Asc(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc)
{
  u8 i,k,n,x0,y0,m=0;
	switch(zk_num)
	{
		// n:�ַ���ռ�ֽ���  d���ּ��
		case 7:   n=7;  x0=5;  y0=7;  break;	 //	  5x7 ASCII
		case 8:   n=8;  x0=7;  y0=8;  break;	 //   7x8 ASCII
		case 12:  n=12; x0=6;  y0=12; break;	 //  6x12 ASCII
	  case 16:  n=16; x0=8;  y0=16; break;	 //  8x16 ASCII	
	  case 24:  n=48; x0=12; y0=24; break;	 //  12x24 ASCII
	 	case 32:  n=64; x0=16; y0=32; break;	 //  16x32 ASCII
	}
	LCD_Address_Set(x,y,x+x0-1,y+y0-1);
	for(i=0;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(fc);
			}
			else
			{
			  LCD_WR_DATA(bc);
			}
			m++;
			if(m%x0==0)
			{
				m=0;
				break;
			}
		}
	}
}

/******************************************************************************
      ����˵������ʾASCII��
      ������ݣ�x,y      д�������
                zk_num   1:5*7   2:7*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Asc_String(u16 x,u16 y,u16 zk_num,u8 text[],u16 fc,u16 bc)
{
	u8 i= 0;
	u8 AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	u32 FontAddr=0; //�ֵ�ַ
	u32 BaseAdd=0; //�ֿ����ַ	
  u8 n,d;// ��ͬ�����ֿ�ļ������
	switch(zk_num)
	{
		//n������d:�ּ��
		case 7:   BaseAdd=0x1DDF80; n=8;  d=6;  break;	 //	  5x7 ASCII
		case 8:   BaseAdd=0x1DE280; n=8;  d=7;  break;	 //   7x8 ASCII
		case 12:  BaseAdd=0x1DBE00; n=12; d=6;  break;	 //  6x12 ASCII
	  case 16:  BaseAdd=0x1DD780; n=16; d=8;  break;	 //  8x16 ASCII	
	  case 24:  BaseAdd=0x1DFF00; n=48; d=12; break;	 //  12x24 ASCII
	 	case 32:  BaseAdd=0x1E5A50; n=64; d=16; break;	 //  16x32 ASCII
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Asc(x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
		}
    i++;  //�¸�����
		x+=d;//��һ�������� 
	}
}


/******************************************************************************
      ����˵������ʾASCII��(Arial&Times New Roman)
      ������ݣ�x,y      д�������
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Arial_TimesNewRoman(u16 x,u16 y,u8 zk_num,u16 fc,u16 bc)
{
  u8 i,k,n,x0,y0,m=0;
	switch(zk_num)
	{
		// n:�ַ���ռ�ֽ���  d:�ּ��
		case 12:  n=26;  x0=10; y0=12; break;	 //  8x12 ASCII
	  case 16:  n=34;  x0=12; y0=16; break;	 //  12x16 ASCII
	  case 24:  n=74;  x0=20; y0=24; break;	 //  16x24 ASCII
	 	case 32:  n=130; x0=25; y0=32; break;	 //  24x32 ASCII
	}
	LCD_Address_Set(x,y,x+x0-1,y+y0-1);
	for(i=2;i<n;i++)
	{
		for(k=0;k<8;k++)
		{
			if((FontBuf[i]&(0x80>>k)))
			{
			  LCD_WR_DATA(fc);
			}
			else
			{
			  LCD_WR_DATA(bc);
			}
			m++;
			if(m%x0==0)
			{
				m=0;
				break;
			}
		}
	}
}



/******************************************************************************
      ����˵������ʾASCII(Arial����)
      ������ݣ�x,y      д�������
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_Arial_String(u16 x,u16 y,u16 zk_num,u8 text[],u16 fc,u16 bc)
{
	u8 i= 0;
	u8 AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	u32 FontAddr=0; //�ֵ�ַ
	u32 BaseAdd=0; //�ֿ����ַ	
  u8 n,d;// ��ͬ�����ֿ�ļ������
	switch(zk_num)
	{
		//n:������d:�ּ��
		case 12:  BaseAdd=0x1DC400; n=26;  d=10; break;	 //  8x12 ASCII(Arial����)
	  case 16:  BaseAdd=0x1DE580; n=34;  d=12; break;	 //  12x16 ASCII(Arial����)
	  case 24:  BaseAdd=0x1E22D0; n=74;  d=20; break;	 //  16x24 ASCII(Arial����)
	 	case 32:  BaseAdd=0x1E99D0; n=130; d=25; break;	 //  24x32 ASCII(Arial����)
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
		}
    i++;  //�¸�����
		x+=d;//��һ�������� 
	}
}


/******************************************************************************
      ����˵������ʾASCII(Arial����)
      ������ݣ�x,y      д�������
                zk_num   1:8*12,  2:12*16,  3:16*24,  4:24*32
                text[]   Ҫ��ʾ���ַ���
                fc ������ɫ
                bc ������ɫ
      ����ֵ��  ��
******************************************************************************/
void Display_TimesNewRoman_String(u16 x,u16 y,u16 zk_num,u8 text[],u16 fc,u16 bc)
{
	u8 i= 0;
	u8 AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	u32 FontAddr=0; //�ֵ�ַ
	u32 BaseAdd=0; //�ֿ����ַ	
  u8 n,d;// ��ͬ�����ֿ�ļ������
	switch(zk_num)
	{
		//n:������d:�ּ��
		case 12:  BaseAdd=0x1DCDC0; n=26;  d=10; break;	 //  8x12 ASCII(TimesNewRoman����)
	  case 16:  BaseAdd=0x1DF240; n=34;  d=12; break;	 //  12x16 ASCII(TimesNewRoman����)
	  case 24:  BaseAdd=0x1E3E90; n=74;  d=20; break;	 //  16x24 ASCII(TimesNewRoman����)
	 	case 32:  BaseAdd=0x1ECA90; n=130; d=25; break;	 //  24x32 ASCII(TimesNewRoman����)
	}
	while((text[i]>0x00))
	{	
	  if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		  FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x,y,zk_num,fc,bc);/*��ʾһ��ascii��LCD�� */
		}
    i++;  //�¸�����
		x+=d;//��һ�������� 
	}
}

