#include "sensors.h"
#include "config.h"
#include "bluetooth.h"

extern void saveSettings();

int sensorPins[8] = {S0,S1,S2,S3,S4,S5,S6,S7};

int sensorMin[8];
int sensorMax[8];
int sensorThreshold = 200;

int weights[8] = {-1000,-700,-400,-200,200,400,700,1000};

static bool manualSensActive = false;
static unsigned long manualSensStart = 0;
static unsigned long manualSensNext = 0;
static long manualSum = 0;
static int manualCount = 0;

int readLine(){

    long sum = 0;
    long count = 0;

    for(int i=0;i<8;i++){

        int v = analogRead(sensorPins[i]);

        if(v > sensorThreshold){
            sum += weights[i];
            count += 1; // Исправлено: прибавляем 1, а не 1000
        }

    }

    if(count == 0) return 4000;
    if(count == 8) return 5000; // Исправлено: 8 датчиков видят линию = перекрёсток

    return sum / count;
}

void startManualSensitivity(){
    manualSensActive = true;
    manualSensStart = millis();
    manualSensNext = manualSensStart + 500;
    manualCount = 0;
    manualSum = 0;
    blePrint("SENS_MANUAL START");
    blePrint("Averages will be sent every 500 ms for 10 sec");
    blePrint("Send SENS=value at any time to save threshold");
}

void manualSensitivityTick(){
    if(!manualSensActive) return;

    for(int i=0;i<8;i++){
        manualSum += analogRead(sensorPins[i]);
    }
    manualCount++;

    unsigned long now = millis();

    if(now >= manualSensNext){
        int avg = 0;
        if(manualCount > 0) {
            avg = manualSum / (manualCount * 8);
        }
        blePrint(String(avg));
        manualCount = 0;
        manualSum = 0;
        manualSensNext += 500;
    }

    if(now - manualSensStart >= 10000){
        manualSensActive = false;
        blePrint("SENS_MANUAL DONE");
        blePrint("Send SENS=value to save threshold");
    }
}

void setSensorThreshold(int threshold){
    sensorThreshold = constrain(threshold, 0, 4095);
    blePrint("SENS=" + String(sensorThreshold) + " OK");
}

void printSensorThreshold(){
    blePrint("SensorThreshold=" + String(sensorThreshold));
}

void calibrateSensors(){

    blePrint("CALIBRATION START");

    for(int i=0;i<8;i++){
        sensorMin[i] = 4095;
        sensorMax[i] = 0;
    }

    for(int k=0;k<200;k++){

        for(int i=0;i<8;i++){

            int v = analogRead(sensorPins[i]);

            if(v < sensorMin[i]) sensorMin[i] = v;
            if(v > sensorMax[i]) sensorMax[i] = v;

        }

        delay(10);
    }

    blePrint("CALIBRATION DONE");
    saveSettings();
    blePrint("CALIBRATION SAVED");
}

void printSensors(){
    String output = "";
    for(int i=0;i<8;i++){
        int reading = analogRead(sensorPins[i]);
        output += String(reading) + " ";
        Serial.print(reading);
        Serial.print(" ");
    }
    Serial.println();
    blePrint(output);
}