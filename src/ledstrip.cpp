#include "ledstrip.h"
#include "config.h"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 1

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastUpdateTime = 0;
unsigned long pulseCounter = 0;

void ledStripInit() {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
    pixels.setPixelColor(0, pixels.Color(r, g, b));
    pixels.show();
}

void updateLED(RobotState state) {
    unsigned long currentTime = millis();
    
    switch(state) {
        case STATE_IDLE: {
            // Плавно пульсирует зелёный (0-255 за 1 сек)
            uint8_t brightness = (uint8_t)(127.5 + 127.5 * sin(2.0 * 3.14159 * currentTime / 1000.0));
            setLEDColor(0, brightness, 0);
            break;
        }
        
        case STATE_RUNNING:
            // Горит зелёным
            setLEDColor(0, 255, 0);
            break;
        
        case STATE_LINE_LOST:
            // Горит жёлтым
            setLEDColor(255, 255, 0);
            break;
        
        case STATE_BATTERY_LOW: {
            // Мигает жёлтым (500мс вкл/выкл)
            if((currentTime / 500) % 2 == 0)
                setLEDColor(255, 255, 0);
            else
                setLEDColor(0, 0, 0);
            break;
        }
        
        case STATE_BATTERY_CRITICAL: {
            // Мигает красным (300мс вкл/выкл)
            if((currentTime / 300) % 2 == 0)
                setLEDColor(255, 0, 0);
            else
                setLEDColor(0, 0, 0);
            break;
        }
    }
}
