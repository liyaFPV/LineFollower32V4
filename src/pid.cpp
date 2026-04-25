#include "pid.h"

float P = 10.0;
float I = 0.0;
float D = 10.0;

float integral = 0;
float prevErr = 0;

float computePID(int err){

    integral += err;
    integral = constrain(integral,-3000,3000);

    float derivative = err - prevErr;

    float out = P*err + I*integral + D*derivative;

    prevErr = err;

    return constrain(out,-255,255);
}

void resetPID(){
    integral = 0;
}