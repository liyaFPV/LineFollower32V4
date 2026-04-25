#pragma once
#include <Arduino.h>

/* ===== PID CONSTANTS ===== */
const float P = 10.0;
const float I = 0.0;
const float D = 10.0;
const int sensorThreshold = 4000;

const int BaseSpeed = 140;
const int TurboSpeed = 140;
const int ReturnSpeed = 200;
const int timeslep = 150;
const int startTimeSleep = 0;

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
//22 23
//19 18
#define motorL 22
#define motorRL 23

#define motorR 18
#define motorRR 19

/* ===== BUTTON ===== */
#define BTN_START 15

/* ===== SENSOR COUNT ===== */
#define SENSOR_COUNT 8