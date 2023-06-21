/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Remote_Control.h"
#include "bsp_dm4310.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t CANx_SendStdData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len);
uint8_t Data[8]={0,1,2,3,4,5,6,7};
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
char buf[64];
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern TIM_HandleTypeDef htim2;

Motor_MIT_Data_t dm43_mit_t;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM8_Init();
  MX_I2C3_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  MX_DMA_Init();
  MX_USART3_UART_Init();
  remote_control_init();
  
    HAL_GPIO_WritePin(GPIOC, Power_OUT1_EN_Pin|Power_OUT2_EN_Pin|Power_5V_EN_Pin, GPIO_PIN_SET);
    
    can_filter_init();
    
    //使能接收中断和空闲中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);  //receive interrupt
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //idle interrupt
    
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);  //receive interrupt
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  //idle interrupt
    
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);  //receive interrupt
    __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);  //idle interrupt
    
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);  //receive interrupt
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);  //idle interrupt
    
    HAL_GPIO_WritePin(GPIOC, RS485_DIR1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RS485_DIR2_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);    /* 开启PWM通道4 */
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);    /* 开启PWM通道4 */
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);    /* 开启PWM通道4 */
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);    /* 开启PWM通道4 */
    HAL_Delay(2000);// 让电机上电
    start_motor(&hcan1,0x01);
    
    dm43_mit_t.kp_int =0.0f;
    dm43_mit_t.kd_int =0.0f;
    dm43_mit_t.v_int =0.0f;
    dm43_mit_t.p_int =0.0f;
    dm43_mit_t.t_int =0.0f;
    //HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOC, RS485_DIR1_Pin, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart1,"Usart1 Hello!\n",14,100);
      HAL_GPIO_WritePin(GPIOC, RS485_DIR1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, RS485_DIR2_Pin, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart2,"Usart2 Hello!\n",14,100);
      HAL_GPIO_WritePin(GPIOB, RS485_DIR2_Pin, GPIO_PIN_RESET);
      HAL_UART_Transmit(&huart4,"Usart4 Hello!\n",14,100);
      HAL_UART_Transmit(&huart5,"Uart5 Hello!\n",13,100);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
      HAL_Delay(100);
      //CANx_SendStdData(&hcan1,0,Data,8);
      ctrl_motor(&hcan1,1,&dm43_mit_t);
      CANx_SendStdData(&hcan2,1,Data,8);
      if(HAL_GPIO_ReadPin(USER_KEY_GPIO_Port,USER_KEY_Pin) == 0)
      {
          HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);    /* 开启PWM通道4 */
          CDC_Transmit_FS("Hello!\n",7);
      }
          

      
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief  发送标准ID的数据帧
 * @param  hcan     CAN的句柄
 * @param  ID       数据帧ID
 * @param  pData    数组指针
 * @param  Len      字节数0~8
 */
uint8_t CANx_SendStdData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len)
{
  static CAN_TxHeaderTypeDef   Tx_Header;
	
	Tx_Header.StdId=ID;
	Tx_Header.ExtId=0;
	Tx_Header.IDE=0;
	Tx_Header.RTR=0;
	Tx_Header.DLC=Len;
        /*找到空的发送邮箱，把数据发送出去*/
	if(HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK) //
	{
		if(HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX1) != HAL_OK)
		{
			HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, (uint32_t*)CAN_TX_MAILBOX2);
        }
    }
    return 0;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
