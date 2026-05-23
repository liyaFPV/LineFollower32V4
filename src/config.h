#pragma once
#include <Arduino.h>
#include <EEPROM.h>

//config
/*
v2 alifa
P=0.20
I=0.00
D=0.50
BaseSpeed=80
TurboSpeed=100
ReturnSpeed=80
trim=0
centerTolerance=500
straightTime=500
timeslep=10
startTimeSleep=0
delayTime=0
centerRecoverEnabled=1
SensorThreshold=4000
*/


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
#define BATTERY_PIN 13 // ADC pin для измерения напряжения батареи

/* ===== MOTOR PINS (ESP32-S3) ===== */

#define motorR 9
#define motorRR 10
#define motorL 11
#define motorRL 12

/* ===== BUTTON ===== */

#define BTN_START 17

/* ===== WS2812 RGB LED ===== */

#define LED_PIN 48  // Pin для WS2812

/* ===== PWM ===== */

#define PWM_FREQ 20000
#define PWM_RES 8
#define PWM_CH_L 0
#define PWM_CH_R 1

/* ===== SENSOR COUNT ===== */

#define SENSOR_COUNT 8