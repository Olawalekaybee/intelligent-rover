#pragma once

// I2C - ESP32 default I2C pins
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// GPS UART2
#define PIN_GPS_RX 16
#define PIN_GPS_TX 17

// MQ-135 analog input
// GPIO34 is input-only and ADC-capable
#define PIN_MQ135_ADC 34

// Servo PWM
#define PIN_SERVO 23

// Left BTS7960 motor driver
#define PIN_LEFT_LPWM 25
#define PIN_LEFT_RPWM 26
#define PIN_LEFT_LEN  33
#define PIN_LEFT_REN  32

// Right BTS7960 motor driver
#define PIN_RIGHT_LPWM 27
#define PIN_RIGHT_RPWM 14
#define PIN_RIGHT_LEN  18
#define PIN_RIGHT_REN  19

// Battery voltage monitor
// GPIO35 is input-only and ADC-capable
#define PIN_BATTERY_ADC 35

// Status LED
#define PIN_STATUS_LED 2