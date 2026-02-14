# 1. STM32F4 Discovery with IR Infrared Obstacle Avoidance

## 1.1. Deliverables

- We want to get data from the IR Infrared Obstacle Avoidance
- Debug: Test the display action using SWV ITM Data Console.

## 1.2. Hardware Needed

1. STM32F4 Discovery Microcontroller
2. External Component: [IR Infrared Obstacle Avoidance](https://hshop.vn/cam-bien-vat-can-hong-ngoai-v1-2)

## 1.3. References

- https://www.micropeta.com/video72

## 1.4. Hardware Connection

### 1.4.1. Connect Microcontroller with External Component

In STM32CubeIDE: 

| No. | External Component | STM32F4 Discovery |
| --- | --- | --- |
| 1 | GND | GND |
| 2 | VCC | 5V |
| 3 | OUT | PA0 |

## 1.5. Configure .ioc in STM32CubeIDE

- Click connectivity --> Click I2C1
- For I2C select I2C
- Set PA0 as ADC1_IN0

## 1.6. Code

In the while loop at **/\* USER CODE BEGIN WHILE \*/**, add:
```c
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
uint32_t adc_val = HAL_ADC_GetValue(&hadc1); // Voltage value read from the External Component, add calculations to get wanted result
HAL_ADC_Stop(&hadc1);

HAL_Delay(100); // small delay between readings
```

The value returned is greatly impacted by temperature and lighting

Example running in debug mode:
<img width="1920" height="1031" alt="{889ED3E9-AA6F-4A8B-87EB-4052A7F05929}" src="https://github.com/user-attachments/assets/8a497a44-e35a-444c-88da-ed272cecb516" />


