#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    256
#define NUM_ROWS    8
#define NUM_COLUMNS 32

CRGB leds[NUM_LEDS];
int leftColIndex = 16;
int rightColIndex = 16;
int stopColIndex = 16;
int numOn = 8;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  for (int x = 0; x < NUM_COLUMNS; x++) {
    illuminateColumn(x, 0, 0, 0);
  }
  FastLED.show();
}

void loop() {
  stop();
  delay(30);
}

int getCell(int x, int y) {
  if (x % 2 > 0) {
    // odd column
    return (x * 8) + y;
  } else {
    // even column
    return (x * 8) + 7 - y;
  }
}

void illuminateColumn(int x, int r, int g, int b) {
  if (x >= NUM_COLUMNS || x < 0) return;
  for (int y = 0; y < NUM_ROWS; y++) {
    leds[getCell(x, y)] = CRGB(r, g, b);
  }
}

void turnLeft() {
  // right to left
  for (int x = -8; x < (NUM_COLUMNS / 2) + 8; x++) {
    if (x < 0 || x >= NUM_COLUMNS / 2) continue;
    // is column on?
    bool isOn = leftColIndex >= x && leftColIndex <= x + numOn;
    illuminateColumn(x, isOn ? 255 : 0, 0, 0);
  }
  FastLED.show();
  if (leftColIndex-- < -8){
    leftColIndex = (NUM_COLUMNS / 2) + 8;
  }
}

void turnRight() {
  // left to right
  for (int x = (NUM_COLUMNS / 2) - 8; x < NUM_COLUMNS + 8; x++) {
    if (x < NUM_COLUMNS / 2 || x > NUM_COLUMNS) continue;
    // is column on?
    bool isOn = rightColIndex <= x && rightColIndex >= x - numOn;
    illuminateColumn(x, isOn ? 255 : 0, 0, 0);
  }
  FastLED.show();
  if (rightColIndex++ > NUM_COLUMNS + 8){
    rightColIndex = (NUM_COLUMNS / 2) -8;
  }
}

void stop() {
  // right to left
  for (int x = 15; x >= 0; x--) {
    // is column on?
    bool isOn = stopColIndex < (NUM_COLUMNS / 2) - x;
    illuminateColumn(x + 16, isOn ? 255 : 0, 0, 0);
    illuminateColumn(16 - x, isOn ? 255 : 0, 0, 0);
  }
  FastLED.show();
  if (stopColIndex > 0){
    stopColIndex--;
  }
}
