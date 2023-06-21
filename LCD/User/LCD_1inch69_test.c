#include "image.h"
#include "LCD_Test.h"
#include "LCD_1in69.h"
#include "DEV_Config.h"

extern TIM_HandleTypeDef htim1;

void LCD_1in69_test()
{

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    LCD_1IN69_SetBackLight(900);
    LCD_1IN69_Init(HORIZONTAL);
    

    Paint_NewImage(LCD_1IN69_WIDTH, LCD_1IN69_HEIGHT+60, 0, WHITE);


    Paint_SetClearFuntion(LCD_1IN69_Clear);
    Paint_SetDisplayFuntion(LCD_1IN69_DrawPoint);

    LCD_1IN69_Clear(WHITE);

    //Paint_SetRotate(180);
    Paint_DrawString_EN(30, 0, "123", &Font24, YELLOW, RED);  
    Paint_DrawString_EN(30, 34, "ABC", &Font24, BLUE, CYAN);
    
    Paint_DrawFloatNum (30, 58, 987.654321,3, &Font12, WHITE, BLACK);
    Paint_DrawString_CN(100, 30, "¥Ô√Óø∆ºº", &Font24CN, WHITE, BLUE);


    
    Paint_DrawImage(gImage_adm, 20, 100, 180, 176);

//    Paint_DrawRectangle(185, 10, 285, 58, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    Paint_DrawLine(185, 10, 285, 58, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
//    Paint_DrawLine(285, 10, 185, 58, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
//    
//    Paint_DrawCircle(120, 60, 25, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    Paint_DrawCircle(150, 60, 25, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    Paint_DrawCircle(190, 60, 25, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    Paint_DrawCircle(145, 85, 25, YELLOW, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    Paint_DrawCircle(165, 85, 25, GREEN, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
//    
    //DEV_Delay_ms(3000);

    //DEV_Module_Exit();
}

