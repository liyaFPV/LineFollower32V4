#include "bluetooth.h"
#include "pid.h"
#include "sensors.h"
#include "motors.h"
#include "battery.h"

#include <NimBLEDevice.h>

extern int BaseSpeed;
extern int TurboSpeed;
extern int ReturnSpeed;
extern int trim;
extern int timeslep;
extern bool robotRun;
extern int startTimeSleep;
extern int dTime;
extern int centerTolerance;
extern int straightTime;
extern bool centerRecoverEnabled;

extern float dividerTop;
extern float dividerBottom;

static bool deviceConnected = false;
static bool btIntroShown = false;
String rxValue = "";

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

NimBLECharacteristic *pCharacteristic;

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
        deviceConnected = true;
        btIntroShown = false;
    }

    void onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
        deviceConnected = false;
        pServer->getAdvertising()->start();
    }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc* desc) {

        std::string value = pCharacteristic->getValue();

        if (value.length() > 0) {

            rxValue = "";

            for (int i = 0; i < value.length(); i++) {
                rxValue += (char)value[i];
            }

            rxValue.trim();

            parse(rxValue);
        }
    }
};

void blePrint(String text){

    if(!deviceConnected) return;

    pCharacteristic->setValue(text.c_str());
    pCharacteristic->notify();
}

void printHelp(){

blePrint("===== LINE FOLLOWER =====");

blePrint("AT COMMANDS:");

blePrint("AT -> ping");
blePrint("START");
blePrint("STOP");

blePrint("P=value");
blePrint("I=value");
blePrint("D=value");

blePrint("B=value (base speed)");
blePrint("T=value (turbo speed)");
blePrint("RS=value (return speed)");
blePrint("TR=value (trim rate)");
blePrint("TS=value (time sleep)");

blePrint("CT=value (centerTolerance)");
blePrint("ST=value (straightTime)");

blePrint("R1=value (divider top resistor, ohms)");
blePrint("R2=value (divider bottom resistor, ohms)");
blePrint("VOLT -> read battery voltage");

blePrint("CAL");
blePrint("SENS -> start manual sensitivity measurement (200 ms average output)");
blePrint("SENS=value -> set sensitivity threshold and save");
blePrint("CONF");
blePrint("SAVE");

blePrint("SS=value (start time sleep)");
blePrint("DT=value (delay time)");
blePrint("CRE=value (centerRecoverEnabled)");
blePrint("=========================");
}

void printConfig(){

blePrint("CONFIG:");

blePrint("P=" + String(P));
blePrint("I=" + String(I));
blePrint("D=" + String(D));

blePrint("BaseSpeed=" + String(BaseSpeed));
blePrint("TurboSpeed=" + String(TurboSpeed));
blePrint("ReturnSpeed=" + String(ReturnSpeed));
blePrint("trim=" + String(trim));
blePrint("centerTolerance=" + String(centerTolerance));
blePrint("straightTime=" + String(straightTime));
blePrint("timeslep=" + String(timeslep));
blePrint("startTimeSleep=" + String(startTimeSleep));
blePrint("delayTime=" + String(dTime));
blePrint("R1=" + String(dividerTop));
blePrint("R2=" + String(dividerBottom));
blePrint("VOLT=" + String(getBatteryVoltage()));
blePrint("RUN=" + String(robotRun));
blePrint("centerRecoverEnabled=" + String(centerRecoverEnabled));
}

void parse(String cmd){

cmd.trim();

if(cmd=="AT") blePrint("OK");

else if(cmd=="START"){
robotRun=true;
blePrint("RUN");
}

else if(cmd=="STOP"){
robotRun=false;
stopMotors();
blePrint("STOP");
}

else if(cmd.startsWith("P=")){
P=cmd.substring(2).toFloat();
}

else if(cmd.startsWith("I=")){
I=cmd.substring(2).toFloat();
}

else if(cmd.startsWith("D=")){
D=cmd.substring(2).toFloat();
}

else if(cmd.startsWith("B=")){
BaseSpeed=cmd.substring(2).toInt();
}

else if(cmd.startsWith("T=")){
TurboSpeed=cmd.substring(2).toInt();
}

else if(cmd.startsWith("TR=")){
trim=cmd.substring(3).toInt();
}
else if(cmd.startsWith("TS=")){
timeslep=cmd.substring(3).toInt();
}
else if(cmd.startsWith("RS=")){
ReturnSpeed=cmd.substring(3).toInt();
}
else if(cmd.startsWith("SS=")){
startTimeSleep=cmd.substring(3).toInt();
}
else if(cmd.startsWith("DT=")){
dTime=cmd.substring(3).toInt();
}
else if(cmd.startsWith("CT=")){
centerTolerance=cmd.substring(3).toInt();
}
else if(cmd.startsWith("ST=")){
straightTime=cmd.substring(3).toInt();
}
else if(cmd.startsWith("R1=")){
dividerTop=cmd.substring(3).toFloat();
}
else if(cmd.startsWith("R2=")){
dividerBottom=cmd.substring(3).toFloat();
}
else if(cmd=="VOLT"){
    blePrint("VOLT=" + String(getBatteryVoltage()));
}
else if(cmd.startsWith("CRE=")){
if(cmd.substring(4).toInt()==1){
    centerRecoverEnabled=true;
}else if (cmd.substring(4).toInt()==0){
    centerRecoverEnabled=false;
}

}
else if(cmd=="CAL"){
calibrateSensors();
}
else if(cmd=="SENS"){
startManualSensitivity();
}

else if(cmd.startsWith("SENS=")){
int value = cmd.substring(5).toInt();
setSensorThreshold(value);
saveSettings();
blePrint("SAVED");
}

else if(cmd=="CONF"){
printConfig();
printSensorThreshold();
}

else if(cmd=="SAVE"){
saveSettings();
blePrint("SAVED");
}

else if(cmd=="HELP"){
printHelp();
}

}

void btInit(){

NimBLEDevice::init("LineFollower");

NimBLEServer *pServer = NimBLEDevice::createServer();
pServer->setCallbacks(new ServerCallbacks());

NimBLEService *pService = pServer->createService(SERVICE_UUID);

pCharacteristic = pService->createCharacteristic(
                     CHARACTERISTIC_UUID,
                     NIMBLE_PROPERTY::READ |
                     NIMBLE_PROPERTY::WRITE |
                     NIMBLE_PROPERTY::NOTIFY
                   );

pCharacteristic->createDescriptor("2902");
pCharacteristic->setCallbacks(new CharacteristicCallbacks());

pService->start();

NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
pAdvertising->addServiceUUID(SERVICE_UUID);
pAdvertising->start();

btIntroShown = false;
}

void btTick(){

    if(deviceConnected && !btIntroShown) {
        blePrint("READY");
        printHelp();
        printConfig();
        printSensorThreshold();
        btIntroShown = true;
    }

    manualSensitivityTick();
}