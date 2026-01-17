#include "drv8833.h"
#include "ENCODER.h"
#include "mpu6050.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>


// Khai báo extern cho các timer do CubeMX tạo trong tim.c
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

float angle = 0.0f; // biến toàn cục lưu góc quay

// Khởi tạo PWM và Encoder
void DRV8833_Init(void)
{
    // PWM right Motor : TIM2 CH2, CH3
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    // PWM left Motor : TIM1 CH2, CH4
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    // Left Encoder: TIM3
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    // Right Encoder: TIM4
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

void lockInterruptDisable_TIM3(void) {
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
}

void lockInterruptEnable_TIM3(void) {
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void lockInterruptDisable_TIM4(void) {
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
}

void lockInterruptEnable_TIM4(void) {
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void resetGyroAngle(void) {
    angle = 0.0f;
}

// yaw_rate: tốc độ góc (°/s) từ MPU6050
// dt: khoảng thời gian giữa 2 lần đọc (s)
void updateGyroAngle(float yaw_rate, float dt) {
    angle += yaw_rate * dt;
}

// Callback SysTick (mặc định gọi mỗi 1 ms)
void HAL_SYSTICK_Callback(void) {
    MPU6050_Raw Raw;
    MPU6050_Read_Data(&Raw);

    // dt = 0.001s vì SysTick = 1ms
    updateGyroAngle(Raw.Gz, 0.001f);
}

// Right Motor
void RightMotor_SetSpeed(int speed_percent)
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

void RightMotor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
}

// Left Motor
void LeftMotor_SetSpeed(int speed_percent)
{
    uint32_t duty = (abs(speed_percent) * __HAL_TIM_GET_AUTORELOAD(&htim1)) / 100;
    if (speed_percent >= 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, duty);
    }
}

void LeftMotor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
}

void Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
}

// Hàm rẽ phải 90 độ
void rightTurn(void)
{
    // Tạm khóa interrupt để tránh bị ngắt khi đang quay
    lockInterruptDisable_TIM3();

    Motor_Stop();
    resetGyroAngle();
    resetLeftEncoder();

    // Quay phải: bánh trái tiến, bánh phải lùi
    RightMotor_SetSpeed(-100);
    LeftMotor_SetSpeed(100);

    uint32_t encoder_value = MAX_ENCODER_VALUE;

    // Điều kiện dừng: khi angle đạt ~90 hoặc encoder đạt số xung tương ứng
    while(angle < 90.0f && encoder_value > (MAX_ENCODER_VALUE - 6650)) {
        setLeftEncoderValue(TIM3->CNT);
        encoder_value = getLeftEncoderValue();
        // angle đã được cập nhật liên tục trong ngắt TIM11

    }

    Motor_Stop();
    HAL_Delay(300);

    lockInterruptEnable_TIM3();
}

// Hàm rẽ trái 90 độ
void leftTurn(void)
{
    lockInterruptDisable_TIM4();

    Motor_Stop();
    resetGyroAngle();
    resetRightEncoder();

    // Quay trái: bánh trái lùi, bánh phải tiến
    RightMotor_SetSpeed(100);
    LeftMotor_SetSpeed(-100);

    uint32_t encoder_value = MAX_ENCODER_VALUE;

    while(angle > -90.0f && encoder_value > (MAX_ENCODER_VALUE - 6650)) {
        setRightEncoderValue(TIM4->CNT);
        encoder_value = getRightEncoderValue();
    }

    Motor_Stop();
    HAL_Delay(300);

    lockInterruptEnable_TIM4();
}
