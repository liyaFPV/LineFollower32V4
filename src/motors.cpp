#include "motors.h"
#include "config.h"

void motorsInit() {

    // LEFT FORWARD
    ledcSetup(PWM_CH_LF, PWM_FREQ, PWM_RES);
    ledcAttachPin(motorL, PWM_CH_LF);

    // LEFT BACKWARD
    ledcSetup(PWM_CH_LB, PWM_FREQ, PWM_RES);
    ledcAttachPin(motorRL, PWM_CH_LB);

    // RIGHT FORWARD
    ledcSetup(PWM_CH_RF, PWM_FREQ, PWM_RES);
    ledcAttachPin(motorR, PWM_CH_RF);

    // RIGHT BACKWARD
    ledcSetup(PWM_CH_RB, PWM_FREQ, PWM_RES);
    ledcAttachPin(motorRR, PWM_CH_RB);
}

void setMotor(int L, int R) {

    L = constrain(L, -255, 255);
    R = constrain(R, -255, 255);

    // LEFT MOTOR
    if (L >= 0) {
        ledcWrite(PWM_CH_LF, L);
        ledcWrite(PWM_CH_LB, 0);
    } else {
        ledcWrite(PWM_CH_LF, 0);
        ledcWrite(PWM_CH_LB, -L);
    }

    // RIGHT MOTOR
    if (R >= 0) {
        ledcWrite(PWM_CH_RF, R);
        ledcWrite(PWM_CH_RB, 0);
    } else {
        ledcWrite(PWM_CH_RF, 0);
        ledcWrite(PWM_CH_RB, -R);
    }
}

void stopMotors() {
    setMotor(0, 0);
}