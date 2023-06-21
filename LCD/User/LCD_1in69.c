/*****************************************************************************
* | File        :   LCD_1in69.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :   Used to shield the underlying layers of each master and enhance portability
*----------------
* | This version:   V1.0
* | Date        :   2023-03-15
* | Info        :   Basic version
 *
 ******************************************************************************/
#include "LCD_1in69.h"
#include "DEV_Config.h"

#include <stdlib.h> //itoa()
#include <stdio.h>

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

LCD_1IN69_ATTRIBUTES LCD_1IN69;
extern SPI_HandleTypeDef hspi1;


/******************************************************************************
function :  Hardware reset
parameter:
******************************************************************************/
static void LCD_1IN69_Reset(void)
{
    LCD_1IN69_RST_1;
    DEV_Delay_ms(100);
    LCD_1IN69_RST_0;
    DEV_Delay_ms(100);
    LCD_1IN69_RST_1;
    DEV_Delay_ms(100);
}

/******************************************************************************
function :  send command
parameter:
     Reg : Command register
******************************************************************************/
static void LCD_WR_REG(uint8_t Reg)
{
    LCD_1IN69_DC_0;
    HAL_SPI_Transmit(&hspi1, &Reg, 1, 100);
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_WR_DATA8(uint8_t Data)
{
    LCD_1IN69_DC_1;
    HAL_SPI_Transmit(&hspi1, &Data, 1, 100);
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_WR_DATA(uint16_t Data)
{
    LCD_1IN69_DC_1;
    uint8_t temp[2];
    temp[0] = Data>>8;
	temp[1] = Data;
    HAL_SPI_Transmit(&hspi1, temp, 2, 100);
}

/******************************************************************************
function :  Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_1IN69_InitReg(void)
{
    
    

    LCD_WR_REG(0x36);    //Memory data access control
    LCD_WR_DATA8(0x00);
	
    LCD_WR_REG(0x3A);    //Interface Pixel Format
    LCD_WR_DATA8(0x05);  //16bit/pixel

    LCD_WR_REG(0xB2);    //Porch Setting
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33); 
    LCD_WR_DATA8(0xB7); 
    LCD_WR_DATA8(0x75); 

    LCD_WR_REG(0xB7);     //Gate Control
    LCD_WR_DATA8(0x35);  

    LCD_WR_REG(0xBB);     //VCOM Setting
    LCD_WR_DATA8(0x3B);

    LCD_WR_REG(0xC0);     //LCM Control
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);     //VDV and VRH Command Enable
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);      //VRH Set
    LCD_WR_DATA8(0x16);   

    LCD_WR_REG(0xC4);      //VDV Set
    LCD_WR_DATA8(0x20);  

    LCD_WR_REG(0xC6);      //Frame Rate Control in Normal Mode
    LCD_WR_DATA8(0x0F);    //60Hz

    LCD_WR_REG(0xD0);      //Power Control 1
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);      //Positive Voltage Gamma Control
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);      //Negative Voltage Gamma Control
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);
    
    LCD_WR_REG(0x2A);      //宽度
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(239);

    
    LCD_WR_REG(0x2B);      //高度
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(239);

    

    LCD_WR_REG(0x21);       //Display Inversion On

    LCD_WR_REG(0x11);       //Sleep Out
	HAL_Delay(30);
    LCD_WR_REG(0x29);       //Display On
    
}



/********************************************************************************
function :  Initialize the lcd
parameter:
********************************************************************************/
void LCD_1IN69_Init(uint8_t Scan_dir)
{
    // Hardware reset
    LCD_1IN69_Reset();

    // Set the resolution and scanning method of the screen
    //LCD_1IN69_SetAttributes(Scan_dir);

    // Set the initialization register
    LCD_1IN69_InitReg();
}

/********************************************************************************
function:   Sets the start position and size of the display area
parameter:
        Xstart  :   X direction Start coordinates
        Ystart  :   Y direction Start coordinates
        Xend    :   X direction end coordinates
        Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_Address_Set(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{

    // set the X coordinates
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(Xstart);
    LCD_WR_DATA(Xend);

    // set the Y coordinates
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(Ystart+20);
    LCD_WR_DATA(Yend+20);

    LCD_WR_REG(0X2C);
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void LCD_1IN69_Clear(uint16_t Color)
{
    uint16_t i,j;
    LCD_Address_Set(0,0,LCD_1IN69_WIDTH-1,LCD_1IN69_HEIGHT-1);
    for(i = 0; i < LCD_1IN69_WIDTH; i++) {
        for(j = 0; j < LCD_1IN69_HEIGHT; j++) {
            LCD_WR_DATA(Color);
        }
     }
}

/******************************************************************************
function :  Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void LCD_1IN69_Display(uint16_t *Image)
{
    uint16_t i,j;

    LCD_Address_Set(0, 0, LCD_1IN69_WIDTH-1, LCD_1IN69_HEIGHT-1);
    
    for(i = 0; i < LCD_1IN69_WIDTH; i++) {
        for(j = 0; j < LCD_1IN69_HEIGHT; j++) {
            DEV_SPI_WRITE((*(Image+i*LCD_1IN69_HEIGHT+j)>>8)&0xff);
            DEV_SPI_WRITE(*(Image+i*LCD_1IN69_WIDTH+j));
        }
    }

}

void LCD_1IN69_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image)
{
    // display
    UDOUBLE Addr = 0;
    uint16_t i,j;
    
    LCD_Address_Set(Xstart, Ystart, Xend , Yend);
    LCD_1IN69_DC_1;
    
    for (i = Ystart; i < Yend - 1; i++) 
    {
        Addr = Xstart + i * LCD_1IN69_WIDTH ;
        for(j=Xstart;j<Xend-1;j++) {
            LCD_WR_DATA(*(Image+Addr+j));
        }
    }
}

void LCD_1IN69_DrawPoint(uint16_t X, uint16_t Y, uint16_t Color)
{
    LCD_Address_Set(X, Y, X, Y);
    LCD_WR_DATA(Color);
}

void LCD_1IN69_SetBackLight(uint16_t Value)
{
    DEV_Set_PWM(Value);
}
