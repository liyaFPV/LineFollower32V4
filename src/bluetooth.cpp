#include "bluetooth.h"
#include "pid.h"
#include "sensors.h"
#include "motors.h"
#include "battery.h"

#include <NimBLEDevice.h>

// ================= EXTERNAL VARS =================

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

// ================= STATE =================

static bool deviceConnected = false;
static bool btIntroShown = false;

static String rxValue;

NimBLECharacteristic* pCharacteristic = nullptr;

// ================= UUID =================

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// ================= BLE PRINT =================

void blePrint(const String& text) {
    if (!deviceConnected || !pCharacteristic) return;

    pCharacteristic->setValue(text.c_str());
    pCharacteristic->notify();
}

// ================= HELP =================

void printHelp() {
    blePrint("===== LINE FOLLOWER =====");
    blePrint("START / STOP");
    blePrint("P I D");
    blePrint("B T RS TR TS");
    blePrint("CT ST");
    blePrint("R1 R2");
    blePrint("VOLT");
    blePrint("CAL / SENS / SAVE");
    blePrint("=========================");
}

// ================= CONFIG =================

void printConfig() {
    blePrint("CONFIG:");

    blePrint("P=" + String(P));
    blePrint("I=" + String(I));
    blePrint("D=" + String(D));

    blePrint("BaseSpeed=" + String(BaseSpeed));
    blePrint("TurboSpeed=" + String(TurboSpeed));
    blePrint("ReturnSpeed=" + String(ReturnSpeed));
    blePrint("trim=" + String(trim));

    blePrint("CT=" + String(centerTolerance));
    blePrint("ST=" + String(straightTime));

    blePrint("TS=" + String(timeslep));
    blePrint("SS=" + String(startTimeSleep));
    blePrint("DT=" + String(dTime));

    blePrint("R1=" + String(dividerTop));
    blePrint("R2=" + String(dividerBottom));

    blePrint("VOLT=" + String(getBatteryVoltage()));
    blePrint("RUN=" + String(robotRun));
    blePrint("CRE=" + String(centerRecoverEnabled));
}

// ================= PARSER =================

void parse(const String& cmd) {

    Serial.println("[BLE RX] " + cmd);

    if (cmd == "AT") blePrint("OK");

    else if (cmd == "START") {
        robotRun = true;
        blePrint("RUN");
    }

    else if (cmd == "STOP") {
        robotRun = false;
        stopMotors();
        blePrint("STOP");
    }

    else if (cmd.startsWith("P=")) P = cmd.substring(2).toFloat();
    else if (cmd.startsWith("I=")) I = cmd.substring(2).toFloat();
    else if (cmd.startsWith("D=")) D = cmd.substring(2).toFloat();

    else if (cmd.startsWith("B=")) BaseSpeed = cmd.substring(2).toInt();
    else if (cmd.startsWith("T=")) TurboSpeed = cmd.substring(2).toInt();
    else if (cmd.startsWith("RS=")) ReturnSpeed = cmd.substring(3).toInt();

    else if (cmd.startsWith("TR=")) trim = cmd.substring(3).toInt();
    else if (cmd.startsWith("TS=")) timeslep = cmd.substring(3).toInt();

    else if (cmd.startsWith("SS=")) startTimeSleep = cmd.substring(3).toInt();
    else if (cmd.startsWith("DT=")) dTime = cmd.substring(3).toInt();

    else if (cmd.startsWith("CT=")) centerTolerance = cmd.substring(3).toInt();
    else if (cmd.startsWith("ST=")) straightTime = cmd.substring(3).toInt();

    else if (cmd.startsWith("R1=")) dividerTop = cmd.substring(3).toFloat();
    else if (cmd.startsWith("R2=")) dividerBottom = cmd.substring(3).toFloat();

    else if (cmd == "VOLT") {
        blePrint("VOLT=" + String(getBatteryVoltage()));
    }

    else if (cmd.startsWith("CRE=")) {
        centerRecoverEnabled = (cmd.substring(4).toInt() == 1);
    }

    else if (cmd == "CAL") calibrateSensors();
    else if (cmd == "SENS") startManualSensitivity();

    else if (cmd.startsWith("SENS=")) {
        setSensorThreshold(cmd.substring(5).toInt());
        saveSettings();
        blePrint("SAVED");
    }

    else if (cmd == "CONF") {
        printConfig();
        printSensorThreshold();
    }

    else if (cmd == "SAVE") {
        saveSettings();
        blePrint("SAVED");
    }

    else if (cmd == "HELP") {
        printHelp();
    }
}

// ================= CALLBACKS =================

class ServerCallbacks : public NimBLEServerCallbacks {

    void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) override {
        deviceConnected = true;
        btIntroShown = false;

        Serial.println("[BLE] CONNECTED");
    }

    void onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) override {
        deviceConnected = false;

        pServer->getAdvertising()->start();

        Serial.println("[BLE] DISCONNECTED -> ADV restarted");
    }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {

    void onWrite(NimBLECharacteristic* characteristic, ble_gap_conn_desc* desc) override {

        std::string value = characteristic->getValue();
        if (value.empty()) return;

        rxValue = "";

        for (char c : value) {
            rxValue += c;
        }

        rxValue.trim();

        if (rxValue.length() > 0) {
            parse(rxValue);
        }
    }
};

// ================= INIT =================

void btInit() {

    Serial.println("\n[BLE] INIT START");

    NimBLEDevice::init("LineFollower");

    // 🔥 IMPORTANT FOR ESP32-S3
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    NimBLEServer* server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    NimBLEService* service = server->createService(SERVICE_UUID);

    pCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ |
        NIMBLE_PROPERTY::WRITE |
        NIMBLE_PROPERTY::NOTIFY
    );

    pCharacteristic->setCallbacks(new CharacteristicCallbacks());

    service->start();

    NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();

    adv->addServiceUUID(SERVICE_UUID);
    adv->setScanResponse(true);

    adv->setMinInterval(0x20);
    adv->setMaxInterval(0x40);

    adv->start();

    Serial.println("[BLE] ADVERTISING STARTED");
    Serial.println("[BLE] SHOULD APPEAR IN NRF CONNECT");
}

// ================= LOOP =================

void btTick() {

    if (deviceConnected && !btIntroShown) {

        Serial.println("[BLE] FIRST CONNECT EVENT");

        blePrint("READY");
        printHelp();
        printConfig();
        printSensorThreshold();

        btIntroShown = true;
    }

    manualSensitivityTick();
}