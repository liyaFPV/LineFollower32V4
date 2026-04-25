#include "config.h"
#include "pid.h"

int lastErr = 0;

bool robotRun=false;
bool started=false;

void run(int err){
    if(err !=4000){
        lastErr=err;
    }
    if(err == 5000){
        setMotor(BaseSpeed,BaseSpeed);
        delay(timeslep);
        return;
    }
    if(err==4000){
        if(lastErr>0)
            setMotor(ReturnSpeed,0);
        else
            setMotor(0,ReturnSpeed);
        return;
    }

    float correction = computePID(err);

    int currentSpeed = (abs(err) < 500) ? TurboSpeed : BaseSpeed;

    int L = currentSpeed + correction;
    int R = currentSpeed - correction;

    setMotor(L,R);
}

void setup(){
    Serial.begin(115200);
    pinMode(BTN_START, INPUT_PULLUP);
    Serial.println("Line Follower 32V4");
}

void loop(){
    if(digitalRead(BTN_START)==LOW){
        robotRun=!robotRun;
        delay(300);
    }
    if(robotRun){
        if(!started){
            setMotor(0,0);
            delay(startTimeSleep);
            started=true;
        }
        int err = readLine();
        run(err);
    }
    else{
        setMotor(0,0);
        started = false;
    }
}