#include "drv8833.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>

// Khai báo extern cho các timer do CubeMX tạo trong tim.c
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

// Biến toàn cục
float omegaA = 0;
float omegaB = 0;

static int32_t encA_last = 0;
static int32_t encB_last = 0;

// Khởi tạo PWM và Encoder
void DRV8833_Init(void)
{
    // PWM Motor A: TIM2 CH2, CH3
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    // PWM Motor B: TIM4 CH3, CH4
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

    // Encoder A: TIM3
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    // Encoder B: TIM1
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}

// Motor A
void MotorA_SetSpeed(int speed_percent)
{
    uint32_t duty = (abs(speed_percent) * __HAL_TIM_GET_AUTORELOAD(&htim2)) / 100;
    if (speed_percent >= 0) {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty);
    }
}

void MotorA_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
}

// Motor B
void MotorB_SetSpeed(int speed_percent)
{
    uint32_t duty = (abs(speed_percent) * __HAL_TIM_GET_AUTORELOAD(&htim4)) / 100;
    if (speed_percent >= 0) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, duty);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty);
    }
}

void MotorB_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
}

// Cập nhật tốc độ góc từ encoder
void Update_Encoder_Speeds(void)
{
    int32_t encA = __HAL_TIM_GET_COUNTER(&htim3);
    int32_t encB = __HAL_TIM_GET_COUNTER(&htim1);

    int32_t dA = encA - encA_last;
    int32_t dB = encB - encB_last;
    encA_last = encA;
    encB_last = encB;

    float dt = SAMPLE_TIME_MS / 1000.0f;
    omegaA = (2.0f * M_PI * dA) / (ENCODER_PPR_A * dt);
    omegaB = (2.0f * M_PI * dB) / (ENCODER_PPR_B * dt);
}
