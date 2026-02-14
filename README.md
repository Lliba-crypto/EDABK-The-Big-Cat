# Deliverables 

> **Deadline: Sat 06/12/2025**

## 1. Embedded: LK Measure Dist. Debug OLED

**Purpose:** Measure Dist. from Wall.

**Link:** https://github.com/Squieler/VL53L0X---STM32-HAL/tree/main

- Adjust file `.ioc`
- Add library `.h` and `.c`
- Apply necessary functions from library to `main.c`

Measure Distance directly from Wall and display value in the OLED.

Testing using Wall at C7-E515/Yours.

<p align="center">
    <img src="pic/pic1.png" width="500">
    <br>
    <em>8 Cases of Wall(s) in 1 Cell</em>
</p>

Upload the Codes in GitHub: With carefully notes on these 8 cases.
- List ALL applied functions in `main.c`

## 2. Embedded: LK Control Motor Debug OLED

**Purpose:** Go.

**Link:** https://github.com/gautam-dev-maker/mushak/tree/main

- Adjust file `.ioc`
- Add library `.h` and `.c`
- Apply necessary functions from library to `main.c`

Each-Step Deliverables:
- Wheel must run!
- Discover all cases of wheel (CW/CCW, speed,...) based on the APPLIED functions in `main.c`
- Wheel must run in all cases discovered as above!

Upload the Codes in GitHub: With carefully notes on ALL cases.
- List ALL applied functions in `main.c`

## 3. Embedded: LK Measure Angle Debug OLED

**Purpose:** Known if turning left/right 90 degree?

**Link:** https://blog.embeddedexpert.io/?p=633

- Adjust file `.ioc`
- Add library `.h` and `.c`
- Apply necessary functions from library to `main.c`

Measure Angle based on DEFAULT Angle from the START.

=> Display in OLED

Upload the Codes in GitHub: With carefully notes on ALL possible ANGLES.
- List ALL applied functions in `main.c`

## 4. CAD for Micromouse

*Coming Soon...*

## 5. Embedded: Merge all 5 things

1. LK Measure Dist.
2. LK Control Motor.
3. LK Measure Angle.
4. OLED
5. Algorithm!

Test at the C7-E515!
- **Stage 1: Discover Mode:** Micromouse at the Start Base => Discover all the possible cells! => Stop at the current cell.
    - BFS Algorithm has completed adjusting its values.
- **Stage 2: Get Back:** At the current cell, get back to the Start Base
- **Stage 3: Goal:** From the Start Base to the Goal.

Upload the Codes in GitHub: With carefully notes on ALL cases.
