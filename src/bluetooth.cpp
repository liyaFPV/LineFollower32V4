#include "bluetooth.h"
#include "pid.h"
#include "sensors.h"
#include "motors.h"
#include <BluetoothSerial.h>

extern int BaseSpeed;
extern int TurboSpeed;
extern int ReturnSpeed;
extern int trim;
extern int timeslep;
extern bool robotRun;
extern int startTimeSleep;
extern int dTime;


BluetoothSerial SerialBT;
static bool btIntroShown = false;

void printHelp(){

SerialBT.println("===== LINE FOLLOWER =====");

SerialBT.println("AT COMMANDS:");

SerialBT.println("AT -> ping");
SerialBT.println("START");
SerialBT.println("STOP");

SerialBT.println("P=value");
SerialBT.println("I=value");
SerialBT.println("D=value");

SerialBT.println("B=value (base speed)");
SerialBT.println("T=value (turbo speed)");
SerialBT.println("RS=value (return speed)");
SerialBT.println("TR=value (trim rate)");
SerialBT.println("TS=value (time sleep)");
SerialBT.println("SS=value (start time sleep)");
SerialBT.println("DT=value (delay time)");

SerialBT.println("CAL");
SerialBT.println("SENS -> start manual sensitivity measurement (200 ms average output)");
SerialBT.println("SENS=value -> set sensitivity threshold and save");
SerialBT.println("CONF");
SerialBT.println("SAVE");
SerialBT.println("=========================");
}

void printConfig(){

SerialBT.println("CONFIG:");

SerialBT.print("P="); SerialBT.println(P);
SerialBT.print("I="); SerialBT.println(I);
SerialBT.print("D="); SerialBT.println(D);

SerialBT.print("BaseSpeed=");
SerialBT.println(BaseSpeed);

SerialBT.print("TurboSpeed=");
SerialBT.println(TurboSpeed);

SerialBT.print("ReturnSpeed=");
SerialBT.println(ReturnSpeed);

SerialBT.print("trim=");
SerialBT.println(trim);

SerialBT.print("timeslep=");
SerialBT.println(timeslep);

SerialBT.print("startTimeSleep=");
SerialBT.println(startTimeSleep);

SerialBT.print("delayTime=");
SerialBT.println(dTime);

SerialBT.print("RUN=");
SerialBT.println(robotRun);

}

void parse(String cmd){

cmd.trim();

if(cmd=="AT") SerialBT.println("OK");

else if(cmd=="START"){
robotRun=true;
SerialBT.println("RUN");
}

else if(cmd=="STOP"){
robotRun=false;
stopMotors();
SerialBT.println("STOP");
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
SerialBT.println("SAVED");
}

else if(cmd=="CONF"){
printConfig();
printSensorThreshold();
}

else if(cmd=="SAVE"){
saveSettings();
SerialBT.println("SAVED");
}

else if(cmd=="HELP"){
printHelp();
}

}

void btInit(){

SerialBT.begin("LineFollower");
btIntroShown = false;
}

void btTick(){

    if(SerialBT.hasClient() && !btIntroShown) {
        SerialBT.println("READY");
        printHelp();
        printConfig();
        printSensorThreshold();
        btIntroShown = true;
    }

    manualSensitivityTick();

    if(!SerialBT.available()) return;

    String cmd=SerialBT.readStringUntil('\n');

    parse(cmd);

    SerialBT.print("> ");

}