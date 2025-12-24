#ifndef __DRV8833_H
#define __DRV8833_H

#include "main.h"
#include <stdint.h>

// Cấu hình encoder
#define ENCODER_PPR_A   13   // pulses per revolution motor A
#define ENCODER_PPR_B   13   // pulses per revolution motor B
#define SAMPLE_TIME_MS  100  // thời gian lấy mẫu

// Biến toàn cục để theo dõi trong Live Expressions
extern float omegaA;   // vận tốc góc motor A (rad/s)
extern float omegaB;   // vận tốc góc motor B (rad/s)

// Khởi tạo driver
void DRV8833_Init(void);

// Hàm điều khiển motor
void MotorA_SetSpeed(int speed_percent); // -100..+100
void MotorB_SetSpeed(int speed_percent); // -100..+100
void MotorA_Stop(void);
void MotorB_Stop(void);

// Hàm cập nhật encoder
void Update_Encoder_Speeds(void);

#endif /* __DRV8833_H */
