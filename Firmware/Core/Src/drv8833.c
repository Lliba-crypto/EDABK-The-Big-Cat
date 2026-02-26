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
#define ENCODER_PPR 817.0f // <--- CON SỐ ĐO BẰNG THỰC NGHIỆM
#define TICKS_PER_MM (ENCODER_PPR / WHEEL_CIRCUMFERENCE) // Hệ số: 11.82 xung/mm
//Một ô mê cung Micromouse tiêu chuẩn có kích thước là 180x180mm.
//Với hệ số TICKS_PER_MM vừa tính ở trên (khoảng 11.82), để đi được 180mm, vi điều khiển cần đếm được: 180 * 11.82 = 2128 xung.

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

// Hàm đánh thức DRV8833
void DRV8833_Wakeup(void) {
    // Kéo chân PB2 lên mức CAO (3.3V)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

    // Theo datasheet, DRV8833 cần tối đa 1ms để sạc tụ bơm điện (charge pump) và khởi động
    HAL_Delay(1);
}

// Hàm ru ngủ DRV8833 (Dùng khi cần tiết kiệm pin)
void DRV8833_Sleep(void) {
    // Kéo chân PB2 xuống mức THẤP (0V)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}

// Khởi tạo PWM và Encoder
void DRV8833_Init(void)
{
	// Đánh thức IC trước khi làm việc khác
	DRV8833_Wakeup();

    // PWM right Motor : TIM2 CH2, CH3
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    // PWM left Motor : TIM1 CH2, CH4
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

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

// Hàm dừng động cơ
void Motor_Stop(void)
{
    /* * TẠI SAO LẠI DÙNG 4199 (MAX_PWM) MÀ KHÔNG PHẢI LÀ 0?
     * * 1. NẾU DÙNG 0 (IN1 = 0, IN2 = 0): CHẾ ĐỘ THẢ TRÔI (Coast Mode)
     * - IC DRV8833 ngắt kết nối điện hoàn toàn khỏi động cơ.
     * - Động cơ mất lực nhưng vẫn tiếp tục quay tự do theo quán tính cơ học.
     * - Hậu quả: Xe sẽ bị trôi lố qua vạch 180mm (Encoder đếm vọt lên rất cao).
     * * 2. NẾU DÙNG 4199 (IN1 = 1, IN2 = 1): CHẾ ĐỘ PHANH ĐIỆN TỪ (Brake Mode)
     * - IC DRV8833 nối tắt (đoản mạch) 2 cọc của động cơ lại với nhau.
     * - Động cơ DC lúc này biến thành một máy phát điện. Việc nối tắt khiến
     * dòng điện cảm ứng tự sinh ra từ trường ngược chiều, ghì chặt rotor lại.
     * - Lợi ích: Bánh xe bị khóa cứng ngay lập tức, quãng đường phanh bằng 0.
     * Giúp robot Micromouse dừng chính xác từng milimet tại tâm ô mê cung.
     */

    // Phanh khóa cứng bánh phải (TIM2)
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 4199);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 4199);

    // Phanh khóa cứng bánh trái (TIM1)
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 4199);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 4199);
}

// Hàm di chuyển thẳng 1 ô (180mm) có chống lệch
void moveOneCell(void) {
    // 1. Reset bộ đếm về 0
    TIM3->CNT = 0;
    TIM4->CNT = 0;
    resetGyroAngle(); // Đưa góc MPU về 0 (nếu bạn muốn dùng kết hợp)

    int target_ticks = 1024; // Mục tiêu 180mm; (1027 + 1022) / 2
    int current_ticks_L = 0;
    int current_ticks_R = 0;
    int average_ticks = 0;
    
    // Tốc độ cơ bản
    int base_speed = 40;

    // Hệ số bù lệch (Kp). Nếu xe vẫn lệch, tăng số này lên (ví dụ 0.5, 1.0, 2.0...)
    float Kp = 0.5f;

    // LẤY MỐC THỜI GIAN BẮT ĐẦU CHẠY
        uint32_t start_time = HAL_GetTick();

        while (average_ticks < target_ticks) {

            // --- PHAO CỨU SINH (TIMEOUT) ---
            // Nếu xe chạy quá 4 giây (4000ms) mà vẫn chưa tới đích (do kẹt tường, hết pin...)
            // -> Ép vòng lặp phải kết thúc để cứu hệ thống không bị đơ!
            if (HAL_GetTick() - start_time > 4000) {
                break;
            }

        // Vẫn giữ MPU cập nhật liên tục để không bị treo
       /* if (flag_10ms_mpu == 1) {
            flag_10ms_mpu = 0;
            MPU6050_Read_Data(&Raw); 
        } */

        // Đọc số xung hiện tại của 2 bánh
        current_ticks_L = (int16_t)TIM3->CNT;
        current_ticks_R = (int16_t)TIM4->CNT;
        average_ticks = (current_ticks_L + current_ticks_R) / 2;

        // --- BẮT ĐẦU BỘ ĐIỀU KHIỂN ĐỒNG BỘ ENCODER ---
        // Tính độ lệch: Nếu L quay nhanh hơn R -> error DƯƠNG
        // Nếu L quay chậm hơn R -> error ÂM
        int error = current_ticks_L - current_ticks_R;

        // Tính lượng tốc độ cần bù
        int turn = (int)(Kp * error);

        // Áp dụng bù trừ: Bánh nào quay nhanh hơn sẽ bị trừ bớt tốc độ
        int speed_L = base_speed - turn;
        int speed_R = base_speed + turn;

        // Khâu bão hòa (Tránh cấp tốc độ quá 100 hoặc số âm)
        if (speed_L > 100) speed_L = 100;
        if (speed_L < 10)  speed_L = 10; // Không cho phép dừng hẳn khi đang đi
        if (speed_R > 100) speed_R = 100;
        if (speed_R < 10)  speed_R = 10;

        // Xuất tốc độ mới ra động cơ
        //LeftMotor_SetSpeed(speed_L);
        //RightMotor_SetSpeed(speed_R);
        // ----------------------------------------------

    /*        // Đọc cảm biến tường
            uint16_t dL = readRangeSingleMillimeters(&sensorLeft, &distanceStr3);
            uint16_t dR = readRangeSingleMillimeters(&sensorRight, &distanceStr2);

            // Chạy PID bù trừ bánh xe để giữ thẳng ở giữa 2 tường
           WallFollow_Update(dL, dR);
    */
     }
    // Đã đi đủ 180mm -> Phanh khóa bánh
    Motor_Stop();
    
    // Dừng lại nghỉ 1 giây để quán tính tắt hẳn trước khi đi ô tiếp theo
    HAL_Delay(1000);

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
