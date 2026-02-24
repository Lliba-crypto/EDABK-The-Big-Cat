#include "drv8833.h"
#include "encoder_motor.h"
#include "mpu6050.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>
#include "vl53l0x.h"

// Báo cho trình biên dịch biết các biến này đã được cấp phát ở main.c
extern VL53L0X_Dev_t sensorLeft;
extern VL53L0X_Dev_t sensorRight;
extern statInfo_t_VL53L0X distanceStr2;
extern statInfo_t_VL53L0X distanceStr3;

// Báo cho trình biên dịch biết hàm này nằm ở main.c
extern void WallFollow_Update(uint16_t distL, uint16_t distR);

#define PI 3.14159f
#define WHEEL_DIAMETER 22.0f
#define WHEEL_CIRCUMFERENCE (PI * WHEEL_DIAMETER) // 69.115 mm
#define ENCODER_PPR 1440.0f // <--- CẦN BẠN CUNG CẤP SỐ NÀY
#define TICKS_PER_MM (ENCODER_PPR / WHEEL_CIRCUMFERENCE)

// Khai báo extern cho các timer do CubeMX tạo trong tim.c
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

float angle = 0.0f; // biến toàn cục lưu góc quay

// Lấy cờ từ TIM6 ở main.c sang
extern volatile uint8_t flag_10ms_mpu;
// Lấy biến Raw từ main.c sang để lát nữa đọc MPU trong lúc rẽ
extern MPU6050_Raw Raw;

// Khởi tạo PWM và Encoder
void DRV8833_Init(void)
{
    // PWM right Motor : TIM2 CH2, CH3
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    // PWM left Motor : TIM1 CH2, CH3
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    // Left Encoder: TIM3
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);

    // Right Encoder: TIM4
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
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
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty);
    }
}

void LeftMotor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
}

void Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
}

//Ham di chuyen thang 1 o
void moveOneCell(void) {
    // 1. Reset bộ đếm Encoder của TIM3 (Trái) và TIM4 (Phải) về 0
    TIM3->CNT = 0;
    TIM4->CNT = 0;

    // 2. Số xung mục tiêu cho 180mm
    // Tạm dùng số lý thuyết cho động cơ 500 RPM (Tỷ số truyền 1:30)
    int target_ticks = 2187;
    
    int current_ticks_L = 0;
    int current_ticks_R = 0;
    int average_ticks = 0;
    
    // 3. Vòng lặp chờ: Vừa bám tường, vừa đếm quãng đường
    while (average_ticks < target_ticks) {
        // --- THÊM ĐOẠN NÀY ĐỂ GIỮ IMU LUÔN SỐNG ---
        if (flag_10ms_mpu == 1) {
            flag_10ms_mpu = 0;
            MPU6050_Read_Data(&Raw); 
        }
        // Đọc giá trị thanh ghi (Ép kiểu int16_t đề phòng trường hợp đếm ngược từ 65535 về 0 do đấu ngược dây pha)
        current_ticks_L = abs((int16_t)TIM3->CNT);
        current_ticks_R = abs((int16_t)TIM4->CNT);

        // Lấy trung bình cộng quãng đường của 2 bánh
        average_ticks = (current_ticks_L + current_ticks_R) / 2;

        // Đọc cảm biến tường
        uint16_t dL = readRangeSingleMillimeters(&sensorLeft, &distanceStr3);
        uint16_t dR = readRangeSingleMillimeters(&sensorRight, &distanceStr2);
        
        // Chạy PID bù trừ bánh xe để giữ thẳng ở giữa 2 tường
        WallFollow_Update(dL, dR);
    }
    
    // 4. Đã đi đủ 180mm -> Cắt động cơ (Phanh hãm)
    LeftMotor_SetSpeed(0);
    RightMotor_SetSpeed(0);

    // Tạm dừng 0.2s để triệt tiêu quán tính cơ học trôi trượt trước khi đi ô tiếp theo
    HAL_Delay(200);
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
    RightMotor_SetSpeed(-60);
    LeftMotor_SetSpeed(60);

    // Reset lại bộ đếm của TIM3 (Bánh trái đang chạy tiến)
    TIM3->CNT = 0;

        // Vòng lặp dừng khi 1 trong 2 điều kiện đạt ngưỡng
        // 1. Gyro báo đủ 87 độ (Điều kiện chính)
        // 2. HOẶC Encoder đếm vượt quá 900 xung (Điều kiện backup chống xoay vòng tròn)
        while(angle < 87.0f && abs((int16_t)TIM3->CNT) < 900) {
            if (flag_10ms_mpu == 1) {
                flag_10ms_mpu = 0;
                MPU6050_Read_Data(&Raw); // Cập nhật angle
            }
        }
    Motor_Stop();
    HAL_Delay(300);
    lockInterruptEnable_TIM3();
}

// LÀM TƯƠNG TỰ CHO HÀM leftTurn() ...

// Hàm rẽ trái 90 độ
void leftTurn(void)
{
    lockInterruptDisable_TIM4();

    Motor_Stop();
    resetGyroAngle();
    resetRightEncoder();

    // Quay trái: bánh trái lùi, bánh phải tiến
    RightMotor_SetSpeed(60);
    LeftMotor_SetSpeed(-60);

    // Reset lại bộ đếm của TIM3 (Bánh trái đang chạy tiến)
        TIM4->CNT = 0;

        // Vòng lặp dừng khi 1 trong 2 điều kiện đạt ngưỡng
        // 1. Gyro báo đủ 87 độ (Điều kiện chính)
        // 2. HOẶC Encoder đếm vượt quá 900 xung (Điều kiện backup chống xoay vòng tròn)
        while(angle > -87.0f && abs((int16_t)TIM4->CNT) < 900) {

            if (flag_10ms_mpu == 1) {
                flag_10ms_mpu = 0;
                MPU6050_Read_Data(&Raw); // Cập nhật angle
            }
        }
    Motor_Stop();
    HAL_Delay(300);

    lockInterruptEnable_TIM4();
}
