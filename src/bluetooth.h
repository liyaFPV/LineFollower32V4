#pragma once
#include <Arduino.h>

void btInit();
void btTick();

void blePrint(const String& text);
void parse(const String& cmd);

void printHelp();
void printConfig();

void saveSettings();