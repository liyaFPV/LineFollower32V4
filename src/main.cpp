#include "config.h"
#include "sensors.h"
#include "pid.h"
#include "motors.h"
#include "bluetooth.h"

int BaseSpeed = 140;
int TurboSpeed = 140;
int ReturnSpeed = 200;
int trim = 0;
int timeslep = 10;
int startTimeSleep=0;
int dTime=0;
bool robotRun = false;
bool started=false;

bool centerRecoverEnabled = false;
bool wasCentered = false;
int centerTolerance = 300;   // допуск центра
int straightTime = 100;      // сколько ехать прямо после потери
int oldmillis=0;
int lastErr = 0;
static const uint32_t EEPROM_MAGIC = 0xA5A55A5A;

void saveSettings() {
    int addr = 0;

    EEPROM.put(addr, EEPROM_MAGIC); addr += sizeof(uint32_t);
    EEPROM.put(addr, P); addr += sizeof(float);
    EEPROM.put(addr, I); addr += sizeof(float);
    EEPROM.put(addr, D); addr += sizeof(float);
    EEPROM.put(addr, BaseSpeed); addr += sizeof(int);
    EEPROM.put(addr, TurboSpeed); addr += sizeof(int);
    EEPROM.put(addr, ReturnSpeed); addr += sizeof(int);
    EEPROM.put(addr, sensorThreshold); addr += sizeof(int);
    EEPROM.put(addr, trim); addr += sizeof(int);
    EEPROM.put(addr, timeslep); addr += sizeof(int);
    EEPROM.put(addr, startTimeSleep); addr += sizeof(int);
    EEPROM.put(addr, dTime); addr += sizeof(int);
    EEPROM.put(addr, centerTolerance); addr += sizeof(int);
    EEPROM.put(addr, straightTime); addr += sizeof(int);
    EEPROM.put(addr, centerRecoverEnabled); addr += sizeof(bool);


    for(int i = 0; i < 8; i++) {
        EEPROM.put(addr, sensorMin[i]); addr += sizeof(int);
    }
    for(int i = 0; i < 8; i++) {
        EEPROM.put(addr, sensorMax[i]); addr += sizeof(int);
    }

    EEPROM.commit();
}

void loadSettings() {
    int addr = 0;
    uint32_t magic = 0;

    EEPROM.get(addr, magic);

    if(magic != EEPROM_MAGIC) {
        for(int i = 0; i < 8; i++) {
            sensorMin[i] = 0;
            sensorMax[i] = 4095;
        }
        sensorThreshold = 200;
        trim = 0;
        timeslep = 10;
        ReturnSpeed = 200;
        startTimeSleep = 0;
        dTime = 0;
        return;
    }

    addr += sizeof(uint32_t);
    EEPROM.get(addr, P); addr += sizeof(float);
    EEPROM.get(addr, I); addr += sizeof(float);
    EEPROM.get(addr, D); addr += sizeof(float);
    EEPROM.get(addr, BaseSpeed); addr += sizeof(int);
    EEPROM.get(addr, TurboSpeed); addr += sizeof(int);
    EEPROM.get(addr, ReturnSpeed); addr += sizeof(int);
    EEPROM.get(addr, sensorThreshold); addr += sizeof(int);
    EEPROM.get(addr, trim); addr += sizeof(int);
    EEPROM.get(addr, timeslep); addr += sizeof(int);
    EEPROM.get(addr, startTimeSleep); addr += sizeof(int);
    EEPROM.get(addr, dTime); addr += sizeof(int);
    EEPROM.get(addr, centerTolerance); addr += sizeof(int);
    EEPROM.get(addr, straightTime); addr += sizeof(int);
    EEPROM.get(addr, centerRecoverEnabled); addr += sizeof(bool);
    

    for(int i = 0; i < 8; i++) {
        EEPROM.get(addr, sensorMin[i]); addr += sizeof(int);
    }
    for(int i = 0; i < 8; i++) {
        EEPROM.get(addr, sensorMax[i]); addr += sizeof(int);
    }
}

void setup(){
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    loadSettings();

    pinMode(BTN_START, INPUT_PULLUP);

    motorsInit();
    btInit();
    delay(1000);
}

void processLine(int err) {
    if(err != 4000 && err != 5000){
        lastErr = err;
        wasCentered = false; 
    }

    if(err == 5000){
        setMotor(BaseSpeed, BaseSpeed);
        delay(timeslep);
        return;
    }
    
    if(err == 4000){
        // 1. ЛОГИКА РАЗРЫВА С ПЛАВНЫМ ПОИСКОМ (Скан ~180 градусов)
        if(centerRecoverEnabled && !wasCentered && abs(lastErr) <= centerTolerance) {
            int searchSpeed = 110; // Пониженная скорость для плавности поиска

            // Плавный поворот ВЛЕВО (примерно на 90 градусов от центра)
            setMotor(-searchSpeed, searchSpeed); 
            long startSearch = millis();
            while(millis() - startSearch < 400) { // Увеличили время до 400мс
                if(readLine() != 4000) return; // Нашли линию — выходим
                btTick(); // Чтобы Bluetooth не отвалился во время цикла
            }

            // Плавный поворот ВПРАВО (на 180 градусов назад)
            setMotor(searchSpeed, -searchSpeed);
            startSearch = millis();
            while(millis() - startSearch < 800) { // 800мс чтобы пройти дугу 180
                if(readLine() != 4000) return;
                btTick();
            }

            // Если ничего не нашли — возвращаемся в центр и едем прямо
            setMotor(-searchSpeed, searchSpeed); // Короткий доверт в центр
            delay(400); 
            
            setMotor(BaseSpeed, BaseSpeed);
            delay(straightTime);
            wasCentered = true;
            return;
        }

        // 2. ЛОГИКА ЗИГЗАГА (Резкий возврат, если вылетели сбоку)
        if(lastErr > 0)
            setMotor(ReturnSpeed, -ReturnSpeed); 
        else
            setMotor(-ReturnSpeed, ReturnSpeed);

        return;
    }

    // ОБЫЧНОЕ ДВИЖЕНИЕ
    float correction = computePID(err + trim);
    int L = BaseSpeed + correction;
    int R = BaseSpeed - correction;
    setMotor(L, R);
}

void loop(){
    btTick();
    if(digitalRead(BTN_START)==LOW){
        robotRun=!robotRun;
        delay(300);
    } 
    if(robotRun){
        int err = readLine();
        processLine(err);
    }
}