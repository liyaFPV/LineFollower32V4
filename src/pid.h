#include <Arduino.h>
#include "config.h"

float computePID(int err);
void resetPID();
void motorsInit();
void setMotor(int L,int R);
void moveTest();
int readLine();