/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "map_track_alg.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "drv8833.h"
#include "mpu6050.h"
#include "encoder_motor.h"
#include "vl53l0x.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RIGHT_MOTOR_Pin GPIO_PIN_1
#define RIGHT_MOTOR_GPIO_Port GPIOA
#define RIGHT_MOTORA2_Pin GPIO_PIN_2
#define RIGHT_MOTORA2_GPIO_Port GPIOA
#define DRV_SLEEP_Pin GPIO_PIN_2
#define DRV_SLEEP_GPIO_Port GPIOB
#define RIGHT_SCREEN_Pin GPIO_PIN_10
#define RIGHT_SCREEN_GPIO_Port GPIOB
#define RIGHT_SCREENB11_Pin GPIO_PIN_11
#define RIGHT_SCREENB11_GPIO_Port GPIOB
#define LEFT_Pin GPIO_PIN_9
#define LEFT_GPIO_Port GPIOC
#define LEFTA8_Pin GPIO_PIN_8
#define LEFTA8_GPIO_Port GPIOA
#define LEFT_MOTOR_Pin GPIO_PIN_9
#define LEFT_MOTOR_GPIO_Port GPIOA
#define LFFT_MOTOR_Pin GPIO_PIN_11
#define LFFT_MOTOR_GPIO_Port GPIOA
#define START_BUTTON_Pin GPIO_PIN_12
#define START_BUTTON_GPIO_Port GPIOA
#define LEFT_ENCODER_Pin GPIO_PIN_4
#define LEFT_ENCODER_GPIO_Port GPIOB
#define LEFT_ENCODERB5_Pin GPIO_PIN_5
#define LEFT_ENCODERB5_GPIO_Port GPIOB
#define RIGHT_ENCODER_Pin GPIO_PIN_6
#define RIGHT_ENCODER_GPIO_Port GPIOB
#define RIGHT_ENCODERB7_Pin GPIO_PIN_7
#define RIGHT_ENCODERB7_GPIO_Port GPIOB
#define FRONT_IMU_Pin GPIO_PIN_8
#define FRONT_IMU_GPIO_Port GPIOB
#define FRONT_IMUB9_Pin GPIO_PIN_9
#define FRONT_IMUB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
