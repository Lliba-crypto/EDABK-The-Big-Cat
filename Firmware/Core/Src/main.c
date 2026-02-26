/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
volatile uint8_t flag_10ms_mpu = 0;
mouse_type mouse;
cell_type maze[MAZE_SIZE][MAZE_SIZE];
int stage = 0;
/* Ground-truth maze */
//cell_type REAL_MAZE[MAZE_SIZE][MAZE_SIZE] = INIT_REAL_MAZE;
VL53L0X_Dev_t sensorLeft;
VL53L0X_Dev_t sensorFront;
VL53L0X_Dev_t sensorRight;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void WallFollow_Update(uint16_t distL, uint16_t distR);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Implement-able Variables for Real-life Application
MPU6050_Data Angle; // use Angle.Yaw for yaw angle
uint16_t distance1, distance2, distance3;

// For IDE Debugging
static char textBuffer[64];   // for formatted strings printed in IDE

// Unused Variables
#define FLT_TEXT_SIZE (1 + 1 + 1 +    8   + 1 + 1 + 5   + 1) // for OLED
statInfo_t_VL53L0X distanceStr1, distanceStr2, distanceStr3;
MPU6050_Raw Raw;
static volatile int counter = 0;
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  MPU6050_Init();
  DRV8833_Init();
  // BẬT NGẮT TIMER 6 ĐỂ ĐỌC MPU MỖI 10ms
    HAL_TIM_Base_Start_IT(&htim6);
  // --- BẮT ĐẦU RADAR QUÉT MPU6050 ---
    HAL_Delay(500);
    ssd1306_Fill(Black);
    ssd1306_SetCursor(2, 10);

    if (HAL_I2C_IsDeviceReady(&hi2c1, 0xD0, 3, 100) == HAL_OK) {
        ssd1306_WriteString("MPU ADDR: 0xD0 OK", Font_7x10, White);
        MPU6050_Init(); // Chỉ khởi tạo khi chắc chắn MPU còn sống
    }
    else if (HAL_I2C_IsDeviceReady(&hi2c1, 0xD2, 3, 100) == HAL_OK) {
        ssd1306_WriteString("MPU ADDR: 0xD2 !!!", Font_7x10, White);
    }
    else {
        ssd1306_WriteString("MPU: NOT FOUND/DEAD", Font_7x10, White);
    }

    ssd1306_UpdateScreen();
    HAL_Delay(3000); // Đứng hình 3 giây để bạn đọc thông báo
    // --- KẾT THÚC RADAR ---
/*  initVL53L0X(&sensorFront, &hi2c1);
  initVL53L0X(&sensorRight, &hi2c2);
  initVL53L0X(&sensorLeft, &hi2c3);
	for(uint8_t i = 0; i < 52; i++) {
		textBuffer[i] = ' ';
	}
	// --- CẤU HÌNH CHO CẢM BIẾN TRƯỚC ---
	setSignalRateLimit(&sensorFront, 0.25);
	setVcselPulsePeriod(&sensorFront, VcselPeriodPreRange, 14);
	setVcselPulsePeriod(&sensorFront, VcselPeriodFinalRange, 10);
	setMeasurementTimingBudget(&sensorFront, 33000);

	// --- CẤU HÌNH CHO CẢM BIẾN PHẢI ---
	setSignalRateLimit(&sensorRight, 0.25);
	setVcselPulsePeriod(&sensorRight, VcselPeriodPreRange, 14);
	setVcselPulsePeriod(&sensorRight, VcselPeriodFinalRange, 10);
	setMeasurementTimingBudget(&sensorRight, 33000);

	// --- CẤU HÌNH CHO CẢM BIẾN TRÁI ---
	setSignalRateLimit(&sensorLeft, 0.25);
	setVcselPulsePeriod(&sensorLeft, VcselPeriodPreRange, 14);
	setVcselPulsePeriod(&sensorLeft, VcselPeriodFinalRange, 10);
	setMeasurementTimingBudget(&sensorLeft, 33000); // 33ms
*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
      while (1)
      {
          // 1. CHỚP ĐÈN NHẸ NHÀNG (Bỏ delay 500ms đi)
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

          // 2. IN THÔNG SỐ LÊN OLED
          int16_t enc_left = (int16_t)TIM3->CNT;
          int16_t enc_right = (int16_t)TIM4->CNT;
          ssd1306_Fill(Black);
          ssd1306_SetCursor(1, 20);
          sprintf(textBuffer, "EncL: %-5d", enc_left);
          ssd1306_WriteString(textBuffer, Font_7x10, White);
          ssd1306_SetCursor(1, 0);
          sprintf(textBuffer, "EncR: %-5d", enc_right);
          ssd1306_WriteString(textBuffer, Font_7x10, White);
          ssd1306_UpdateScreen();

          // (Quá trình vẽ OLED mất khoảng vài chục ms, đủ để tạo delay tự nhiên cho vòng lặp)
          // 3. TẠM KHÓA TOÀN BỘ MPU Ở ĐÂY ĐỂ CÁCH LY LỖI
                /* if (flag_10ms_mpu == 1) {
                    flag_10ms_mpu = 0;
                    MPU6050_Read_Data(&Raw);
                } */

          // 4. KIỂM TRA NÚT BẤM PA12
                if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {

                    HAL_Delay(50); // Lọc nhiễu

                    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {

                        // Bật đèn sáng liên tục báo hiệu đang chạy
                        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

                        moveOneCell(); // Hàm này chạy tối đa 4 giây là bắt buộc phải tự thoát

                        // Đã chạy xong, tắt đèn
                        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

                        // -----------------------------------------------------------
                        // VÒNG LẶP CHỜ NHẢ NÚT (Đã tích hợp máy báo lỗi)
                        // -----------------------------------------------------------
                        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {
                            // KHÔNG ĐỌC MPU Ở ĐÂY NỮA !!!

                            // Chớp đèn cực nhanh để báo hiệu: "Tôi đang kẹt ở nút bấm!"
                            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
                            HAL_Delay(50);
                        }
                        // -----------------------------------------------------------
                        HAL_Delay(50);
                    }
                }

      	// 3. ĐỌC CẢM BIẾN VL53L0X
      /*	distance2 = readRangeSingleMillimeters(&sensorFront, &distanceStr1);
      	distance3 = readRangeSingleMillimeters(&sensorRight, &distanceStr2);
      	distance1 = readRangeSingleMillimeters(&sensorLeft, &distanceStr3);

      	sprintf(textBuffer, "D1:%d D2:%d D3:%d\r\n", distance1, distance2, distance3);

      	// 4. Đọc trạng thái nút PA12 (Chống dội phím Non-blocking)
      	int stage = 1; // Khởi tạo luôn bằng 0
      	static uint32_t last_button_press = 0; // Lưu thời điểm bấm nút cuối cùng
      	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {
      	// Chỉ nhận lệnh nếu khoảng cách giữa 2 lần bấm lớn hơn 200ms
      	  if (HAL_GetTick() - last_button_press > 200) {
      	    stage = (stage + 1) % 4; // chuyển sang chế độ tiếp theo
      	    last_button_press = HAL_GetTick(); // Cập nhật lại mốc thời gian
      	  }
      	}
      	// 5. MÁY TRẠNG THÁI (STATE MACHINE)
      	switch (stage){
      	case 0:
      	  Motor_Stop(); // Standby thì nên dừng động cơ
      	  DRV8833_Sleep(); // Ngắt điện hoàn toàn động cơ, chuột ngủ say sưa
      	  break;
      	case 1:
      	  DRV8833_Wakeup(); // Đánh thức chuột dậy
      	  WallFollow_Update(distance1, distance3);
      	  //explore(maze, &mouse);
      	  break;
      	case 2:
      	  returnToStart(maze, &mouse);
      	  break;
      	case 3:
      	  goToOptimal(maze, &mouse, 8, 8);
      	  break;
      	}
      */
      }
      /* USER CODE END WHILE */
}
/*
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 4199;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4199;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 84-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DRV_SLEEP_GPIO_Port, DRV_SLEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin : DRV_SLEEP_Pin */
  GPIO_InitStruct.Pin = DRV_SLEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DRV_SLEEP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : START_BUTTON_Pin */
  GPIO_InitStruct.Pin = START_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(START_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        flag_10ms_mpu = 1;
    }
}

// Các hệ số PID (Bạn sẽ cần tinh chỉnh thực tế trên sa bàn)
#define Kp 0.6f
#define Kd 0.2f

// Khoảng cách lý tưởng từ tâm robot đến 1 bên tường (Đơn vị: mm)
// Giả sử ô mê cung rộng 180mm, robot rộng 90mm -> Khoảng cách 2 bên là 45mm
// --- THÔNG SỐ VẬT LÝ MÊ CUNG & ROBOT ---
#define TARGET_DIST_SIDE 45   // Khoảng cách lý tưởng tới tường (mm)
#define WALL_THRESHOLD_MM 120 // Ngưỡng xác định có tường (mm)
#define BASE_SPEED 60         // Tốc độ chạy thẳng nền (0-100)

int previous_error = 0;

void WallFollow_Update(uint16_t distL, uint16_t distR) {
    int error = 0;

    // Kiểm tra xem có tường ở các bên không
    uint8_t has_left = (distL < WALL_THRESHOLD_MM);
    uint8_t has_right = (distR < WALL_THRESHOLD_MM);

    // 1. TÍNH TOÁN SAI SỐ (ERROR)
    if (has_left && has_right) {
        error = distL - distR;
        // Nếu lệch trái -> distL nhỏ -> error âm.
        // Nếu lệch phải -> distR nhỏ -> error dương.
    } else if (has_left) {
        error = distL - TARGET_DIST_SIDE;
    } else if (has_right) {
        error = TARGET_DIST_SIDE - distR;
    } else {
        error = 0; // Ngã tư, không có tường để bám
    }

    // 2. TÍNH TOÁN KHÂU P VÀ D
    float P_term = Kp * error;
    float D_term = Kd * (error - previous_error);

    int turn = (int)(P_term + D_term);
    previous_error = error; // Lưu lại cho vòng lặp sau

    // 3. TÍNH TỐC ĐỘ 2 BÁNH TƯƠNG ỨNG
    // Nếu error < 0 (Lệch trái) -> turn < 0
    // -> Bánh trái tăng tốc (-turn), Bánh phải giảm tốc (+turn) để bẻ lái sang phải.
    int speedL = BASE_SPEED - turn;
    int speedR = BASE_SPEED + turn;

    // 4. KHÂU BÃO HÒA (SATURATION)
    // Đảm bảo tốc độ không vượt quá giới hạn PWM của DRV8833 (0-100)
    if (speedL > 100) speedL = 100;
    if (speedL < 0) speedL = 0;

    if (speedR > 100) speedR = 100;
    if (speedR < 0) speedR = 0;

    // 5. XUẤT XUNG RA ĐỘNG CƠ
    LeftMotor_SetSpeed(speedL);
    RightMotor_SetSpeed(speedR);
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
