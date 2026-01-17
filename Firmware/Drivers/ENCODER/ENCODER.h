#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx_hal.h"

// Định nghĩa thông số encoder và bánh xe
#define ENCODER_RESOLUTION   1024        // số xung mỗi vòng encoder
#define WHEEL_DIAMETER_CM    4.4f        // đường kính bánh xe (cm)
#define MAX_ENCODER_VALUE    32767       // giá trị reset (nếu dùng timer 16-bit)

// Khai báo extern các handle timer (được tạo trong CubeMX)
extern TIM_HandleTypeDef htim3;   // encoder trái
extern TIM_HandleTypeDef htim4;   // encoder phải

// Hàm reset encoder
void resetLeftEncoder(void);
void resetRightEncoder(void);

void setLeftEncoderValue(uint32_t value);
uint32_t getLeftEncoderValue(void);
void setRightEncoderValue(uint32_t value);
uint32_t getRightEncoderValue(void);

// Hàm lấy quãng đường đã đi (cm)
float getLeftEncoderDistance(void);
float getRightEncoderDistance(void);
float getAverageDistance(void);

#endif /* __ENCODER_H */
