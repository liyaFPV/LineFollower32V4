#pragma once
#include <Arduino.h>
#include <EEPROM.h>

/* ===== EEPROM ===== */
#define EEPROM_SIZE 512

/* ===== SENSOR PINS (ESP32-S3 ADC1) ===== */
#define S0 1
#define S1 2
#define S2 3
#define S3 4
#define S4 5
#define S5 6
#define S6 7
#define S7 8

#define BATTERY_PIN 13

/* ===== MOTOR PINS ===== */
#define motorR 9
#define motorRR 10
#define motorL 11
#define motorRL 12

/* ===== BUTTON ===== */
#define BTN_START 17

/* ===== WS2812 ===== */
#define LED_PIN 48

/* ===== PWM CONFIG ===== */
#define PWM_FREQ 20000
#define PWM_RES 8

/* PWM CHANNELS (ВАЖНО: 4 канала!) */
#define PWM_CH_LF 0
#define PWM_CH_LB 1
#define PWM_CH_RF 2
#define PWM_CH_RB 3

#define SENSOR_COUNT 8