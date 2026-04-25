#pragma once
#include <Arduino.h>

extern float P;
extern float I;
extern float D;

float computePID(int err);
void resetPID();