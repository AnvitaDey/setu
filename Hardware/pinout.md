# Pinout — ESP32 Dev Module

| Component | ESP32 Pin | Notes |
|---|---|---|
| LCD SDA | GPIO 21 | I2C |
| LCD SCL | GPIO 22 | I2C |
| Push button | GPIO 19 | `INPUT_PULLUP`, active-low |
| Mic module (analog out) | GPIO 34 | ADC, input-only pin |
| Tilt sensor | GPIO 33 | Digital |
| Servo (haptic) | GPIO 18 | PWM via ESP32Servo |

Default I2C address for the LCD backpack is `0x27` — if your display doesn't respond, run an I2C scanner sketch first; some backpacks ship on `0x3F`.
