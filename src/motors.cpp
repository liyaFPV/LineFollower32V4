#include "motors.h"
#include "config.h"

void motorsInit(){
    // Настраиваем 4 канала PWM (по 2 на каждый мотор)
    ledcSetup(0, PWM_FREQ, PWM_RES); // Лево Вперед
    ledcSetup(1, PWM_FREQ, PWM_RES); // Лево Назад
    ledcSetup(2, PWM_FREQ, PWM_RES); // Право Вперед
    ledcSetup(3, PWM_FREQ, PWM_RES); // Право Назад

    ledcAttachPin(motorL, 0);
    ledcAttachPin(motorRL, 1);
    ledcAttachPin(motorR, 2);
    ledcAttachPin(motorRR, 3);
}

void setMotor(int L, int R){
    // Разрешаем диапазон от -255 до 255
    L = constrain(L, -255, 255);
    R = constrain(R, -255, 255);

    // Левый мотор
    if (L >= 0) {
        ledcWrite(0, L);
        ledcWrite(1, 0);
    } else {
        ledcWrite(0, 0);
        ledcWrite(1, -L); 
    }

    // Правый мотор
    if (R >= 0) {
        ledcWrite(2, R);
        ledcWrite(3, 0);
    } else {
        ledcWrite(2, 0);
        ledcWrite(3, -R);
    }
}

void stopMotors(){
    setMotor(0,0);
}