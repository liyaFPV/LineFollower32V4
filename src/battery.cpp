#include "battery.h"

float dividerTop = 100000.0;
float dividerBottom = 100000.0;

void batteryInit() {
    pinMode(BATTERY_PIN, INPUT);
    analogSetAttenuation(ADC_11db);  // Для полного диапазона 0-3.3V
}

int getBatteryVoltage() {
    // ESP32 ADC: 12-bit (0-4095), с отведением 11db может читать до ~3.3V
    int adcValue = analogRead(BATTERY_PIN);
    float adcMv = (adcValue * 3300.0f) / 4095.0f;
    float ratio = (dividerTop + dividerBottom) / dividerBottom;
    float batteryMv = adcMv * ratio;
    return (int)batteryMv;
}

void updateBattery(bool &batteryLow, bool &batteryCritical) {
    int voltage = getBatteryVoltage();
    
    if(voltage <= BATTERY_CRITICAL_MV) {
        batteryCritical = true;
        batteryLow = false;
    } else if(voltage <= BATTERY_LOW_MV) {
        batteryCritical = false;
        batteryLow = true;
    } else {
        batteryCritical = false;
        batteryLow = false;
    }
}
