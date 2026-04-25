#include "motors.h"
#include "config.h"

void motorsInit(){

    ledcSetup(PWM_CH_L,PWM_FREQ,PWM_RES);
    ledcSetup(PWM_CH_R,PWM_FREQ,PWM_RES);

    ledcAttachPin(motorL,PWM_CH_L);
    ledcAttachPin(motorR,PWM_CH_R);

}

void setMotor(int L,int R){

    L = constrain(L,0,255);
    R = constrain(R,0,255);

    ledcWrite(PWM_CH_L,L);
    ledcWrite(PWM_CH_R,R);
}

void stopMotors(){
    setMotor(0,0);
}