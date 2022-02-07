#ifndef Memory_h
#define Memory_h

#include <EEPROM.h>
#include "Lights.h"
#include "Counter.h"

#define RESET_PIN 20

int resetCountdown = 0;

int readMemory()
{
    Serial.println("Reading memory");
    // return 42069420;
    return EEPROM.get(0, counter);
}
void clearMemory()
{
    Serial.println("Clearing memory");
    // return;
    for (int i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.write(i, 0);
    }
    FastLED.clear();
    setCounter(0);
}

void SaveMemory(int counter)
{
    Serial.println("Saving memory");
    EEPROM.put(0, counter);
}

void readReset()
{
    int resetButton = digitalRead(RESET_PIN);
    if (resetButton == LOW)
    {
        resetCountdown++;
        // Serial.println("Reset button pressed");
    }
    else
    {
        resetCountdown = 0;
    }
}

void displayReset()
{
    if (resetCountdown > LED_COUNT)
    {
        clearMemory();
        FastLED.clear();
        resetCounter();
        resetCountdown = 0;
    }

    for (int i = 0; i < resetCountdown; i++)
    {
        leds[i] = CHSV(0, 255, BRIGHTNESS);
    }
}

#endif