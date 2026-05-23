#pragma once
#include <Arduino.h>
#include "config.h"

// Напряжения батареи (в мВ)
#define BATTERY_CRITICAL_MV 3000  // Критическое напряжение (полностью разряжена)
#define BATTERY_LOW_MV 3200        // Низкое напряжение (нужно зарядить)
#define BATTERY_FULL_MV 4200       // Полная батарея

extern float dividerTop;
extern float dividerBottom;

void batteryInit();
void updateBattery(bool &batteryLow, bool &batteryCritical);
int getBatteryVoltage();
