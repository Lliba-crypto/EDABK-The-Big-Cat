#ifndef __DRV8833_H
#define __DRV8833_H

#include "main.h"
#include <stdint.h>

// Cấu hình encoder
#define ENCODER_PPR_A   13   // pulses per revolution motor A
#define ENCODER_PPR_B   13   // pulses per revolution motor B
#define SAMPLE_TIME_MS  100  // thời gian lấy mẫu
#define FORWARD_TIME 200 // thời gian đi thẳng 1 ô

// Biến toàn cục để theo dõi trong Live Expressions
extern float omegaA;   // vận tốc góc motor A (rad/s)
extern float omegaB;   // vận tốc góc motor B (rad/s)

// Biến toàn cục angle
extern float angle;

// Hàm bật/tắt ngắt TIM3
void lockInterruptDisable_TIM3(void);
void lockInterruptEnable_TIM3(void);

// Hàm reset góc gyro
void resetGyroAngle(void);

//hàm tính angle
void updateGyroAngle(float yaw_rate, float dt);

//Hàm đánh thức và ru ngủ
void DRV8833_Wakeup(void);
void DRV8833_Sleep(void);

// Khởi tạo driver
void DRV8833_Init(void);

// Hàm điều khiển motor
void RightMotor_SetSpeed(int speed_percent); // -100..+100
void LeftMotor_SetSpeed(int speed_percent); // -100..+100
void RightMotor_Stop(void);
void LeftMotor_Stop(void);
void Motor_Stop(void);
// Hàm cập nhật encoder
void Update_Encoder_Speeds(void);
//Ham di chuyen 1 o
void moveOneCell(void);
//Hàm rẻ trái, phải
void rightTurn(void);
void leftTurn(void);
#endif /* __DRV8833_H */
