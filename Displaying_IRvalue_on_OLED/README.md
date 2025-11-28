# 1. STM32F4 Discovery with LCD 1602 I2C Display

## 1.1. Deliverables

- We want to display the ADC value to SSD1306
- Debug: Test the display action using SWV ITM Data Console.

## 1.2. Hardware Needed

1. STM32F4 Discovery Microcontroller
2. LK1: [Màn hình OLED SSD1306 trắng](https://hshop.vn/lcd-oled-trang-0-96-inch-i2c)
3. LK2: [Cảm biến hồng ngoại sử dụng LM393](

## 1.3. References

- https://www.youtube.com/watch?v=z1Px6emHIeg&t=345s

## 1.4. Hardware Connection

### 1.4.1. Connect LK1 with Microcontroller

| No. | LK1 | STM32F4 Discovery |
| --- | --- | --- |
| 1 | GND | GND |
| 2 | VCC | 5V |
| 3 | SDA | PB9 |
| 4 | SCL | PB8 |

### 1.4.2. Connect LK2 with Microcontroller

| No. | LK1 | STM32F4 Discovery |
| --- | --- | --- |
| 1 | GND | GND |
| 2 | VCC | 5V |
| 3 | OUT | PA0 |

## 1.5. Configure .ioc in STM32CubeIDE

- Click connectivity --> Click I2C1
- For I2C select I2C
- Set PB8 to I2C1_SCL
- Set PB9 to I2C1_SDA
- Set PA) to ADC1_IN0

### 1.6. Setup IDE

-Click "Properties"
<img width="519" height="1130" alt="Screenshot 2025-11-26 163046" src="https://github.com/user-attachments/assets/61f794e7-e9bf-473c-b605-05cd3b876041" />

-Then find "C/C++ General" -> "Paths and symbols" -> -> "Add"
<img width="823" height="629" alt="Screenshot 2025-11-26 163231" src="https://github.com/user-attachments/assets/28eaca0c-7bed-4a66-9fbb-c045d582381b" />

-In "Add directory path", click "Workspace":
<img width="831" height="550" alt="Screenshot 2025-11-26 163608" src="https://github.com/user-attachments/assets/961cb44a-36f3-4c73-a770-a6b69531e058" />

-Choose "OLED, then click "OK":
<img width="684" height="742" alt="Screenshot 2025-11-26 163737" src="https://github.com/user-attachments/assets/7d809fc0-8dd3-49ab-86d2-1a3e1c762cc6" />

-Click "OK":
<img width="503" height="295" alt="Screenshot 2025-11-26 164023" src="https://github.com/user-attachments/assets/a58ee62e-fd22-4748-9267-4263b3289ceb" />

-Move "/$(ProjName)/Drives/OLED" up to below "Core/Inc", then click "Apply and close":
<img width="822" height="657" alt="Screenshot 2025-11-26 164057" src="https://github.com/user-attachments/assets/6daf6201-4947-4c76-a1f6-fa55fc530570" />

## 1.7. Code

### 1.7.1. Adding Necessary Libraries

- Add ssd1306.c; ssd1306.h; ssd1306_font.c; ssd1306_fonts.h; ssd1306_conf.h inside Driver/OLED folder.
- Add STM32F4xx_HAL_DRIVER inside Driver folder.
- Change main.c at:
    - `/* Includes ------------------------------------------------------------------*/`
      #include "main.h"
      #include "ssd1306.h"
      #include "ssd1306_fonts.h"
      #include "stdio.h"
    - `/* USER CODE BEGIN Includes */`
    - `/* USER CODE BEGIN PV */`
    - `/* USER CODE BEGIN PFP */`

### 1.7.2. What are Needed Declaration & Function for `main.c`?

- Initialize OLED:
    - `ssd1306_Init()`: initialize OLED SSD1306
- Application:
    - `ssd1306_Fill(SSD1306_COLOR color);`: Fill the screen with black color
    - `ssd1306_SetCursor(uint8_t x, uint8_t y);`: Set cursor at (x,y) where it prints out string
    - `ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color);`: Print string 'str' using font and color
    - `ssd1306_UpdateScreen()`: Print string at the buffer onto the screen

## 1.8. Video

https://youtube.com/shorts/FKFbyf-zWnk
