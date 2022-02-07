#ifndef Lights_h
#define Lights_h

#include <FastLED.h>

#define LED_COUNT 240
#define BOX_LED_COUNT 12
#define LED_PIN 1
#define BOX_LED_PIN 10

#define BRIGHTNESS 230

#define LOGFIRE false

#define WEED_MAX 200
#define RAINBOW_MAX 200
int rainbowTimer = RAINBOW_MAX;
int weedTimer = WEED_MAX;
float weedHue = 0;

byte saturation = 255;
byte startColor = 0;

bool rainbowDigits[] = {false, false, false, false,
                        false, false, false, false};
bool weedDigits[] = {false, false, false, false,
                     false, false, false, false};

CRGBArray<LED_COUNT> leds;
CRGBArray<BOX_LED_COUNT> boxleds;
// CRGB leds[LED_COUNT];
// CRGB boxleds[BOX_LED_COUNT];

byte WheelPos = 0;

byte colors[11][3] = {
    {0, 0, BRIGHTNESS},               // white
    {0, saturation, BRIGHTNESS},      // red
    {28, saturation, BRIGHTNESS},     // orange
    {28 * 2, saturation, BRIGHTNESS}, // yellow
    {28 * 3, saturation, BRIGHTNESS}, // l green
    {28 * 4, saturation, BRIGHTNESS}, // green
    {28 * 5, saturation, BRIGHTNESS}, // aqua-ish
    {28 * 6, saturation, BRIGHTNESS}, // blue
    {28 * 7, saturation, BRIGHTNESS}, // purpley
    {28 * 8, saturation, BRIGHTNESS}, // pinkish
    {0, 0, 55},                       // grey
};

byte rainbowColors[11][3] = {
    {23 * 0, saturation, BRIGHTNESS},
    {23 * 1, saturation, BRIGHTNESS},
    {23 * 2, saturation, BRIGHTNESS},
    {23 * 3, saturation, BRIGHTNESS},
    {23 * 4, saturation, BRIGHTNESS},
    {23 * 5, saturation, BRIGHTNESS},
    {23 * 6, saturation, BRIGHTNESS},
    {23 * 7, saturation, BRIGHTNESS},
    {23 * 8, saturation, BRIGHTNESS},
    {23 * 9, saturation, BRIGHTNESS},
    {23 * 10, saturation, BRIGHTNESS},
};

byte digitSegmentDefs[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {1, 0, 0, 0, 0, 1, 0}, // 1
    {0, 1, 1, 0, 1, 1, 1}, // 2
    {1, 1, 0, 0, 1, 1, 1}, // 3
    {1, 0, 0, 1, 0, 1, 1}, // 4
    {1, 1, 0, 1, 1, 0, 1}, // 5
    {1, 1, 1, 1, 1, 0, 1}, // 6
    {1, 0, 0, 0, 1, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 0, 1, 1, 1, 1}  // 9
};

byte segGroups[7][4] = {
    // skip 0
    {1, 2, 3, 4},    // top right
    {5, 6, 7, 8},    // top
    {9, 10, 11, 12}, // top left
    // skip 13
    {14, 15, 16, 17}, // bottom left
    {18, 19, 20, 21}, // bottom
    {22, 23, 24, 25}, // bottom right
    {26, 27, 28, 29}, // center
};

#define FIRETIMER 100
#define FIREWINDOWS 100
int allFireCounts[FIREWINDOWS];
int firecounter = 0;
int fireWindowCount = 0;
unsigned long fireWindowStartMillis = 0;
unsigned long buttonPushedMillis = 0;

int getFireCount()
{
  int totalFire = 0;

  for (int i = 0; i < FIREWINDOWS; i++)
  {

    totalFire += allFireCounts[i];
  }
  if (LOGFIRE)
  {
    Serial.print("Fire subs: ");

    for (int i = 0; i < FIREWINDOWS; i++)
    {
      Serial.print(allFireCounts[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println(totalFire);
  }

  return totalFire;
}

void displayFire()
{
  // // Serial.println("Displaying fire");
  // for (int i = 0; i < BOX_LED_COUNT; i++)
  // {
  //   boxleds[i] = CHSV(0, 200, 200);
  // }
  // FastLED.show();
  // return;

  int count = getFireCount();

  byte color[3] = {int(count / 2.5), 255, 250};

  for (int i = 0; i < BOX_LED_COUNT; i++)
  {
    if (count > 5)
      boxleds[i] = CHSV(color[0], color[1], color[2]);
    else
      boxleds[i] = CHSV(0, 0, 10);
  }
}

void updateFire()
{
  unsigned long currentMillis = millis();
  if (currentMillis - fireWindowStartMillis > FIRETIMER) // defines how long between checks of the windows
  {
    allFireCounts[fireWindowCount] = firecounter; // how many presses in the current window
    firecounter = 0;
    fireWindowCount = (fireWindowCount + 1) % FIREWINDOWS; // this rotates us through the array
    fireWindowStartMillis = currentMillis;
    // Serial.print("Fire count: ");
    // Serial.println(getFireCount());
  }
}

void initLEDs()
{
  Serial.println("Initializing LEDs");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.addLeds<WS2812, BOX_LED_PIN, GRB>(boxleds, BOX_LED_COUNT);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  for (int i = 0; i < FIREWINDOWS; i++)
  {
    allFireCounts[i] = 0;
  }
  fireWindowStartMillis = millis();
  Serial.printf("Firecount: %d ", getFireCount());
  Serial.println();
}

void showSegment(byte segment, byte color, byte segDisplay)
{
  int segmentOffset = segDisplay * 30;
  ;
  for (byte i = 0; i < 4; i++)
  {
    leds[segmentOffset + segGroups[segment][i]] = CHSV(colors[color][0], 255, BRIGHTNESS);
    // leds[(segGroups[segment] + segmentOffset][i]).setHSV(colors[color][0], colors[color][1], colors[color][2]);
  }
}

void showSegmentWithColor(byte segment, byte color[3], byte segDisplay)
{
  int segmentOffset = segDisplay * 30;
  ;

  for (byte i = 0; i < 4; i++)
  {
    leds[segmentOffset + segGroups[segment][i]] = CHSV(color[0], 255, BRIGHTNESS);
  }
}

void showSegmentRainbow(byte segment, byte color, byte segDisplay)
{
  if (color == 10)
  {
    showSegment(segment, color, segDisplay);
    return;
  }
  int segmentOffset = segDisplay * 30;
  int pixelOffset = 0;
  for (byte i = 0; i < 4; i++)
  {

    //horizontal ones are easy
    if (segment == 4)
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[0][0], 255, BRIGHTNESS);
    if (segment == 6)
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[5][0], 255, BRIGHTNESS);
    if (segment == 1)
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[10][0], 255, BRIGHTNESS);

    // vertical ones are fucking stupid
    if (segment == 3)
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[4 - i][0], 255, BRIGHTNESS);
    if (segment == 5)
    { // LEAVE ME ALONE.
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[i + 1][0], 255, BRIGHTNESS);
    }
    if (segment == 2)
    {
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[9 - i][0], 255, BRIGHTNESS);
    }
    if (segment == 0)
      leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[i + 6][0], 255, BRIGHTNESS);
    //    leds[(segGroups[segment+segmentOffset][i] + pixelOffset)].setHSV(rainbowColors[segment][0], rainbowColors[segment][1], rainbowColors[segment][2]);
  }
}

void showSegmentRainbow2(byte segment, byte color, byte segDisplay)
{
  if (color == 10)
  {
    showSegment(segment, color, segDisplay);
    return;
  }
  int segmentOffset = segDisplay * 30;
  int pixelOffset = 0;
  for (byte i = 0; i < 4; i++)
  {
    leds[segmentOffset + segGroups[segment][i]] = CHSV(rainbowColors[0][0], 255, BRIGHTNESS);
  }
}

void showDigit(byte number, byte color, byte segDisplay)
{
  //  Serial.println("showing a digit" );

  for (byte i = 0; i < 7; i++)
  { // this is where it iterates over the segments
    if (digitSegmentDefs[number][i] != 0)
    {
      showSegment(i, color, segDisplay);

      if (rainbowTimer < RAINBOW_MAX)
      {
        if (rainbowDigits[7 - segDisplay])
        {
          // showSegmentRainbow(i, color, segDisplay);
          showSegmentRainbow2(i, color, segDisplay);
        }
      }
      if (weedTimer < WEED_MAX)
      {
        if (weedDigits[7 - segDisplay])
        {
          byte weedColor[3] = {int(weedHue), saturation, BRIGHTNESS};
          showSegmentWithColor(i, weedColor, segDisplay);
        }
      }
    }
  }
}

#endif