# 1. STM32F4 Discovery with LCD 1602 I2C Display

## 1.1. Deliverables

- We want to display the string from microcontroller to LCD 1602
- Debug: Test the display action using SWV ITM Data Console.

## 1.2. Hardware Needed

1. STM32F4 Discovery Microcontroller
2. LK1: [Màn hình LCD text LCD1602 xanh lá](https://hshop.vn/lcd-text-lcd1602-xanh-lo)
3. LK2: [Mạch chuyển giao tiếp LCD1602, LCD1604, LCD2004 sang I2C](https://hshop.vn/mach-chuyen-giao-tiep-lcd1602-lcd1604-lcd2004-sang-i2c)

## 1.3. References

- https://www.micropeta.com/video61

## 1.4. Hardware Connection

### Connect external component with Microcontroller

| No. | LK1+LK2 | STM32F4 Discovery |
| --- | --- | --- |
| 1 | GND | GND |
| 2 | VCC | 5V |
| 3 | SDA | ... |
| 4 | SCL | ... |

## 1.5. Configure .ioc in STM32CubeIDE

- Click connectivity --> Click I2C1
- For I2C select I2C
- Set PB8 to I2C1_SCL
- Set PB9 to I2C1_SDA

## 1.6. Code

**AFTER auto-code-generation from 1.5. Configure .ioc in STM32CubeIDE,**

### 1.6.1. Adding Necessary Libraries

- Add liquidcrystal_i2c.h inside Core/Inc folder.
- Add liquidcrystal_i2c.c inside Core/Inc folder.
- Change main.c at:
    - `/* Includes ------------------------------------------------------------------*/`
    - `/* USER CODE BEGIN Includes */`
    - `/* USER CODE BEGIN PV */`
    - `/* USER CODE BEGIN PFP */`
- ...

### 1.6.2. What are Needed Declaration & Function for `main.c`?

- Initialize LCD:
    - `LiquidCrystal_I2C lcd;`: create a sctruct variable
    - `LCD_Init(&lcd, &hi2c1, 0x27, 16, 2);`: Initialize the LCD: assign handle I2C (hi2c1) -> assign the adress -> set the numbers of columns (16) and rows (2)
- Application:
    - `LCD_Clear(&lcd);`: Delete the whole screen
    - `LCD_Backlight(&lcd);`: Turn on the LED backlight
    - `LCD_SetCursor(&lcd, 0, 0);`: Set the cursor at the given point (example: 0,0 means the left top corner)        
    - `LCD_Print(&lcd, "Hello world!");`: Print at the current cursor

## 1.7. Video


https://drive.google.com/drive/folders/1_tJJjdufixqz3SXSOdm4i1_hiw0CCb4u?usp=sharing 
