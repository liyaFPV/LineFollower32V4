#pragma once
#include <Arduino.h>

enum RobotState {
    STATE_IDLE,              // Ждет команды - пульсирует зелёный
    STATE_RUNNING,           // В заезде - горит зелёным
    STATE_LINE_LOST,         // Потеряна линия - горит жёлтым
    STATE_BATTERY_LOW,       // Подсевший акб - мигает жёлтым
    STATE_BATTERY_CRITICAL   // Разряжен акб - мигает красным
};

void ledStripInit();
void updateLED(RobotState state);
void setLEDColor(uint8_t r, uint8_t g, uint8_t b);
