#pragma once
#include <Arduino.h>

void btInit();
void btTick();
void blePrint(String text);
void printHelp();
void printConfig();
void parse(String cmd);
void saveSettings();