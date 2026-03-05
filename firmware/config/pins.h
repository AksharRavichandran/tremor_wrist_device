#pragma once

// =============================================================
// GPIO PIN ASSIGNMENTS — ESP32 Dev Board
// Adjust if using Arduino Nano (see comments)
// =============================================================

// IMU: MPU6050 uses default I2C bus
// ESP32:  SDA=GPIO21, SCL=GPIO22
// Nano:   SDA=A4,     SCL=A5

// --- Mode Button (active LOW, use INPUT_PULLUP) ---
#define PIN_MODE_BUTTON     15

// --- Vibration Motors (PWM via NPN transistor) ---
#define PIN_VIBRATION_1     25
#define PIN_VIBRATION_2     26   // Optional second motor

// --- RGB LED (common cathode) ---
// NOTE: Do NOT use GPIO21 for LED — conflicts with I2C SDA on ESP32
#define PIN_LED_RED         18
#define PIN_LED_GREEN       19
#define PIN_LED_BLUE        4    // Use GPIO4, not GPIO21

// --- SD Card (SPI) — Optional ---
#define PIN_SD_CS           5
// Default SPI on ESP32: MOSI=23, MISO=19, SCK=18
// NOTE: SCK(18) and MISO(19) conflict with LED pins above if SD enabled
// When using SD, move LED to different pins or use I2C LED driver

// --- PWM Channels (ESP32 LEDC peripheral) ---
#define PWM_CH_MOTOR1       0
#define PWM_CH_MOTOR2       1
#define PWM_CH_LED_R        2
#define PWM_CH_LED_G        3
#define PWM_CH_LED_B        4
#define PWM_FREQ_HZ         1000
#define PWM_RESOLUTION      8    // 8-bit: 0-255
