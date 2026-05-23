#pragma once
#include <Arduino.h>
#include "config.h"

// Напряжения батареи (в мВ)
#define BATTERY_CRITICAL_MV 1500  // Критическое напряжение (полностью разряжена)
#define BATTERY_LOW_MV 1600        // Низкое напряжение (нужно зарядить)
#define BATTERY_FULL_MV 2100       // Полная батарея

extern float dividerTop;
extern float dividerBottom;

void batteryInit();
void updateBattery(bool &batteryLow, bool &batteryCritical);
int getBatteryVoltage();
