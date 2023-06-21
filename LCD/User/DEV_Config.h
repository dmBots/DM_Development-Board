#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_


#include <stdint.h>
#include <stdio.h>
#include "gpio.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/

#define DEV_RST_PIN     GPIOB,GPIO_PIN_4		    //PB4
#define DEV_DC_PIN      GPIOC,GPIO_PIN_1			//PC1
#define DEV_CS_PIN		GPIOA,GPIO_PIN_4			//PA4
#define DEV_BL_PIN		TIM1->CCR1 					//PA8

/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define DEV_Digital_Read(_pin) HAL_GPIO_ReadPin(_pin)

/**
 * SPI
**/
#define DEV_SPI_WRITE(_dat)  DEV_SPI_WRite(_dat);


/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms) HAL_Delay(__xms)

/**
 * PWM_BL
**/

#define DEV_Set_PWM(_Value)     DEV_BL_PIN= _Value

/*-----------------------------------------------------------------------------*/
void DEV_SPI_WRite(UBYTE _dat);
int DEV_Module_Init(void);
void DEV_Module_Exit(void);
#endif
