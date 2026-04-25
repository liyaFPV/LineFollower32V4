#pragma once

extern int sensorMin[8];
extern int sensorMax[8];
extern int sensorThreshold;

extern int sensorPins[8];

int readLine();
void calibrateSensors();
void printSensors();
void startManualSensitivity();
void manualSensitivityTick();
void setSensorThreshold(int threshold);
void printSensorThreshold();