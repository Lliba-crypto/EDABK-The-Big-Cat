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

## 1.6. Debug using SWV ITM Data Console

### 1.6.1. Setup Code

In syscalls.c,
- Adding these line codes https://github.com/niekiran/Embedded-C/blob/master/All_source_codes/target/itm_send_data.c
- In `_write` function, change `__io_putchar(*ptr++);` to `ITM_SendChar(*ptr++);`.

### 1.6.2. Setup IDE

In "Debug Configurations" > "Debugger", enable the Serial Wire Viewer (SWV).

![enable_swv](pic/enable_swv.png)

Then, "Debug As STM32 C/C++ Application".

In the Debug Mode, at the "SWV ITM Data Console", click this:

![swv_conf_1](pic/swv_conf_1.png)

Enable Port 0:

![swv_conf_2](pic/swv_conf_2.png)

Then click this to track printf:

![swv_conf_3](pic/swv_conf_3.png)

Finally, click "Resume":

![resume](pic/resume.png)

The project finally runs like this:

![running_interface](pic/running_interface.png)

### 1.6.3. How to Use?

Just `printf` just like a normal C code. Result will appears in SWV ITM Data Console.

## 1.7. Code

**AFTER auto-code-generation from 1.5. Configure .ioc in STM32CubeIDE,**

### 1.7.1. Adding Necessary Libraries

- Add liquidcrystal_i2c.h inside Core/Inc folder.
- Add liquidcrystal_i2c.c inside Core/Inc folder.
- Change main.c at:
    - `/* Includes ------------------------------------------------------------------*/`
    - `/* USER CODE BEGIN Includes */`
    - `/* USER CODE BEGIN PV */`
    - `/* USER CODE BEGIN PFP */`
- ...

### 1.7.2. What are Needed Declaration & Function for `main.c`?

- Initialize LCD:
    - `LiquidCrystal_I2C lcd;`: ...
    - `LCD_Init(&lcd, &hi2c1, 0x27, 16, 2);`: [Purpose?...]
- Application:
    - `LCD_Clear(&lcd);`: ...
    - `LCD_Backlight(&lcd);`: ...
    - `LCD_SetCursor(&lcd, 0, 0);`: ...
    - `LCD_Print(&lcd, "Hello world!");`: ...

## 1.8. Video


[Insert video here...] as a proof of a successful hardware run.
