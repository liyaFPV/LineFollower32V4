#include "motors.h"
#include "config.h"

void motorsInit(){
    // Настраиваем PWM для моторов (ESP32-S3 новый API)
    ledcAttach(motorL, PWM_FREQ, PWM_RES);    // Лево Вперед
    ledcAttach(motorRL, PWM_FREQ, PWM_RES);   // Лево Назад
    ledcAttach(motorR, PWM_FREQ, PWM_RES);    // Право Вперед
    ledcAttach(motorRR, PWM_FREQ, PWM_RES);   // Право Назад
}

void setMotor(int L, int R){
    // Разрешаем диапазон от -255 до 255
    L = constrain(L, -255, 255);
    R = constrain(R, -255, 255);

    // Левый мотор
    if (L >= 0) {
        ledcWrite(motorL, L);
        ledcWrite(motorRL, 0);
    } else {
        ledcWrite(motorL, 0);
        ledcWrite(motorRL, -L); 
    }

    // Правый мотор
    if (R >= 0) {
        ledcWrite(motorR, R);
        ledcWrite(motorRR, 0);
    } else {
        ledcWrite(motorR, 0);
        ledcWrite(motorRR, -R);
    }
}

void stopMotors(){
    setMotor(0,0);
}