#include "encoder_motor.h"
#include "drv8833.h"
#include <math.h>

// Khai báo extern cho các timer do CubeMX tạo trong tim.c
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

// Biến toàn cục
float omegaA = 0;
float omegaB = 0;

static int32_t encA_last = 0;
static int32_t encB_last = 0;

// Reset encoder trái
void resetLeftEncoder(void) {
    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
    TIM3->CNT = MAX_ENCODER_VALUE;
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

// Reset encoder phải
void resetRightEncoder(void) {
    HAL_TIM_Encoder_Stop(&htim4, TIM_CHANNEL_ALL);
    TIM4->CNT = MAX_ENCODER_VALUE;
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

// Gán giá trị cho encoder trái
void setLeftEncoderValue(uint32_t value) {
    TIM3->CNT = value;
}

// Lấy giá trị hiện tại của encoder trái
uint32_t getLeftEncoderValue(void) {
    return TIM3->CNT;
}

// Gán giá trị cho encoder phải
void setRightEncoderValue(uint32_t value) {
    TIM4->CNT = value;
}

// Lấy giá trị hiện tại của encoder phải
uint32_t getRightEncoderValue(void) {
    return TIM4->CNT;
}

// Cập nhật tốc độ góc từ encoder
void Update_Encoder_Speeds(void)
{
    int32_t encA = __HAL_TIM_GET_COUNTER(&htim3);
    int32_t encB = __HAL_TIM_GET_COUNTER(&htim4);

    int32_t dA = encA - encA_last;
    int32_t dB = encB - encB_last;
    encA_last = encA;
    encB_last = encB;

    float dt = SAMPLE_TIME_MS / 1000.0f;
    omegaA = (2.0f * M_PI * dA) / (ENCODER_PPR_A * dt);
    omegaB = (2.0f * M_PI * dB) / (ENCODER_PPR_B * dt);
}

// Tính quãng đường encoder trái
float getLeftEncoderDistance(void) {
    int32_t count = TIM3->CNT;
    int32_t pulses = count - MAX_ENCODER_VALUE;

    float wheel_circumference = (float)M_PI * WHEEL_DIAMETER_CM;
    float distance = ((float)pulses / ENCODER_RESOLUTION) * wheel_circumference;

    return distance; // cm
}

// Tính quãng đường encoder phải
float getRightEncoderDistance(void) {
    int32_t count = TIM4->CNT;
    int32_t pulses = count - MAX_ENCODER_VALUE;

    float wheel_circumference = (float)M_PI * WHEEL_DIAMETER_CM;
    float distance = ((float)pulses / ENCODER_RESOLUTION) * wheel_circumference;

    return distance; // cm
}

// Lấy trung bình quãng đường 2 bánh
float getAverageDistance(void) {
    float distL = getLeftEncoderDistance();
    float distR = getRightEncoderDistance();
    return (distL + distR) / 2.0f;
}
