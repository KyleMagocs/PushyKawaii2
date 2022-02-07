#define FASTLED_ALLOW_INTERRUPTS 0

#include "Lights.h"
#include "Memory.h"

#define BUTTON_PIN 8
#define RESET_PIN 20

int prevButtonState = 0;

void copyColor(byte *src, byte *dst, int len)
{
  memcpy(dst, src, sizeof(src[0]) * len);
}

void shuffleColors()
{
  for (int i = 0; i < 10; i++)
  {
    int n = random(0, 10);
    //   Serial.print("gonna swap the next two colors:");
    //   Serial.print(i);
    //   Serial.print(n);
    byte temp[3];
    copyColor(colors[n], temp, 3);
    copyColor(colors[i], colors[n], 3);
    copyColor(temp, colors[i], 3);
  }
}

void readButton()
{
  // Serial.println("Reading button");
  int counterButton = digitalRead(BUTTON_PIN);
  if (counterButton != prevButtonState)
  {
    if (counterButton == LOW)
    {
      FastLED.clear();
      incrementCounter();
      firecounter++;
      if (counter % 1000 == 0)
      {
        shuffleColors();
        SaveMemory(counter);
      }

    }
    prevButtonState = counterButton;
  }
}

void displayCounter()
{

  // Serial.println("Displaying counter");
  char counterString[8];
  sprintf(counterString, "%8ld", counter);
  for (unsigned int i = 0; i < 8; i++)
  { // 8 digits
    if ((counterString[7 - i]) != ' ')
    {
      int c = counterString[7 - i] - '0';
      showDigit(c, c, i);
    }
  }
}

void checkRainbow()
{
    for (int i = 0; i < 8; i++)
    {
      rainbowDigits[i] = false;
    }

  char counterString[8];
  sprintf(counterString, "%8ld", counter);
  for (int i = 0; i < 7; i++)
  {
    if (counterString[i] == '6' && counterString[i + 1] == '9')
    {
      rainbowDigits[i] = true;
      rainbowDigits[i + 1] = true;
      rainbowTimer = 0;
    }
  }
}

void checkWeed()
{
    for (int i = 0; i < 8; i++)
    {
      weedDigits[i] = false;
    }

  char counterString[8];
  sprintf(counterString, "%8ld", counter);
  for (int i = 0; i < 6; i++)
  {
    if (counterString[i] == '4' && counterString[i + 1] == '2' && counterString[i + 2] == '0')
    {
      weedDigits[i] = true;
      weedDigits[i + 1] = true;
      weedDigits[i + 2] = true;
      weedTimer = 0;
    }
  }
}

void updateEffects()
{
  // Serial.println("Updating effects");
  checkRainbow();
  checkWeed();

  if (rainbowTimer < RAINBOW_MAX)
  {
    for (int i = 0; i < 11; i++)
      rainbowColors[i][0] = (rainbowColors[i][0] + 1 % 255);
    rainbowTimer++;
  }
  if (weedTimer < WEED_MAX)
  {
    weedHue = 105;
    // weedHue -= 0.25;
    // weedHue = max(weedHue, 24);
    weedTimer++;
  }
}

// borrowed from Adafruit Neopixel library
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// borrowed from Adafruit Neopixel library
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup()
{
  
  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  setCounter(readMemory());
  Serial.begin(57600);
  initLEDs();
  shuffleColors();
  for (unsigned int i = 0; i < 8; i++)
  { // 8 digits

    showDigit(8, i, i);
  }
  FastLED.show();
  delay(2000);
}

void loop()
{
  FastLED.clear();

  readButton();

  displayCounter();
  displayFire();

  readReset();
  displayReset();

  FastLED.show();

  updateEffects();
  updateFire();
}
