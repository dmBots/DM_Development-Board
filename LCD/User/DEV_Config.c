#include "DEV_Config.h"
#include "spi.h"



extern SPI_HandleTypeDef hspi1;


void DEV_delay_ms(uint16_t xms )
{
	HAL_Delay(xms);
}


void DEV_SPI_WRite(uint8_t _dat)
{
    HAL_SPI_Transmit(&hspi1, &_dat, 1, 100);
}

int DEV_Module_Init(void)
{
    DEV_Digital_Write(DEV_DC_PIN, 1);
    DEV_Digital_Write(DEV_RST_PIN, 1);
    
    return 0;
}

void DEV_Module_Exit(void)
{
    DEV_Digital_Write(DEV_DC_PIN, 0);
    //close 
    DEV_Digital_Write(DEV_RST_PIN, 0);
    //HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
}
