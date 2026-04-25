#include "sensors.h"
#include "config.h"
#include <BluetoothSerial.h>

extern BluetoothSerial SerialBT;
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
            count += 1000;
        }

    }

    if(count == 0) return 4000;
    if(count == 8000) return 5000; // Перекрёсток

    return sum / count;
}

void startManualSensitivity(){
    manualSensActive = true;
    manualSensStart = millis();
    manualSensNext = manualSensStart + 500;
    manualCount = 0;
    manualSum = 0;
    SerialBT.println("SENS_MANUAL START");
    SerialBT.println("Averages will be sent every 500 ms for 10 sec");
    SerialBT.println("Send SENS=value at any time to save threshold");
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
        SerialBT.println(avg);
        manualCount = 0;
        manualSum = 0;
        manualSensNext += 500;
    }

    if(now - manualSensStart >= 10000){
        manualSensActive = false;
        SerialBT.println("SENS_MANUAL DONE");
        SerialBT.println("Send SENS=value to save threshold");
    }
}

void setSensorThreshold(int threshold){
    sensorThreshold = constrain(threshold, 0, 4095);
    SerialBT.print("SENS=");
    SerialBT.print(sensorThreshold);
    SerialBT.println(" OK");
}

void printSensorThreshold(){
    SerialBT.print("SensorThreshold=");
    SerialBT.println(sensorThreshold);
}

void calibrateSensors(){

    SerialBT.println("CALIBRATION START");

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

    SerialBT.println("CALIBRATION DONE");
    saveSettings();
    SerialBT.println("CALIBRATION SAVED");
}

void printSensors(){

    for(int i=0;i<8;i++){

        SerialBT.print(analogRead(sensorPins[i]));
        SerialBT.print(" ");

    }

    SerialBT.println();
}