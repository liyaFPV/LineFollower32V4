#include "pid.h"

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

void motorsInit(){
    pinMode(motorL, OUTPUT);
    pinMode(motorRL, OUTPUT);
    pinMode(motorR, OUTPUT);
    pinMode(motorRR, OUTPUT);

}

void setMotor(int L,int R){
    L = constrain(L,-255,255);
    R = constrain(R,-255,255);
    if(L>0){
        analogWrite(motorL, L);
        analogWrite(motorRL, 0);
    }else{
        analogWrite(motorL, 0);
        analogWrite(motorRL, L);
    }
    if(R>0){
        analogWrite(motorR, R);
        analogWrite(motorRR, 0);
    }else{
        analogWrite(motorR, 0);
        analogWrite(motorRR, R);
    }
}

void moveTest(){
    Serial.println("L");
    analogWrite(motorL, 140);
    delay(1000);
    analogWrite(motorL, 0);
    delay(1000);

    Serial.println("LR");
    analogWrite(motorRL, 140);
    delay(1000);
    analogWrite(motorRL, 0);
    delay(1000);

    Serial.println("R");
    analogWrite(motorR, 140);
    delay(1000);
    analogWrite(motorR, 0);
    delay(1000);

    Serial.println("RR");
    analogWrite(motorRR, 140);
    delay(1000);
    analogWrite(motorRR, 0);
    delay(1000);
}

int sensorPins[8] = {S0,S1,S2,S3,S4,S5,S6,S7};
int weights[8] = {-3500,-2500,-1500,-500,500,1500,2500,3500};

int readLine(){

    long sum = 0;
    long count = 0;

    for(int i=0;i<8;i++){

        int v = analogRead(sensorPins[i]);

        if(v > sensorThreshold){
            sum += weights[i];
            count += 1000;
        }

    }

    if(count == 0) return 4000;
    if(count == 8000) return 5000; // Перекрёсток

    return sum / count;
}