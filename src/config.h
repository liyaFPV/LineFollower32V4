#pragma once
#include <Arduino.h>
#include <EEPROM.h>

//config
/*
v1 stable
P=10.00
I=0.00
D=10.00
BaseSpeed=130
TurboSpeed=130
ReturnSpeed=200
trim=0
timeslep=150
startTimeSleep=0
delayTime=0
SensorThreshold=4000
*/


/* ===== EEPROM ===== */
#define EEPROM_SIZE 512

/* ===== SENSOR PINS ===== */

#define S0 36
#define S1 39
#define S2 34
#define S3 35
#define S4 32
#define S5 33
#define S6 25
#define S7 26

/* ===== MOTOR PINS ===== */

#define motorR 22
#define motorRR 23
#define motorL 18
#define motorRL 19

/* ===== BUTTON ===== */

#define BTN_START 15

/* ===== PWM ===== */

#define PWM_FREQ 20000
#define PWM_RES 8
#define PWM_CH_L 0
#define PWM_CH_R 1

/* ===== SENSOR COUNT ===== */

#define SENSOR_COUNT 8