#include <FastLED.h>

#define LED_PIN             7
#define NUM_LEDS            256
#define NUM_ROWS            8
#define NUM_COLUMNS         32
#define RUNNING_LIGHTS_PIN  8
#define LEFT_TURN_PIN       11
#define RIGHT_TURN_PIN      12

CRGB leds[NUM_LEDS];
int leftColIndex = 16;
int rightColIndex = 16;
int stopColIndex = 16;
int numOn = 8;
int runningLightBrightness = 50;
int fullBrightness = 255;
bool wasTurningRight = false;
bool wasTurningLeft = false;
bool wasStopping = false;
bool wasRunningLights = false;
int loopCounter = 0;


void setup() {
  pinMode(RUNNING_LIGHTS_PIN, INPUT);
  pinMode(LEFT_TURN_PIN, INPUT);
  pinMode(RIGHT_TURN_PIN, INPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
  for (int x = 0; x < NUM_COLUMNS; x++) {
    illuminateColumn(x, 0, 0, 0);
  }

  FastLED.show();

  Serial.begin(9600);
  Serial.println("Arduino is ready");
}

void loop() {
  // read pins every 5 loops
  bool isTurningLeft = wasTurningLeft;
  bool isTurningRight = wasTurningRight;
  bool isStopping = wasStopping;
  bool isRunningLights = wasRunningLights;

  if (loopCounter++ == 10) {
    int runningLights = digitalRead(RUNNING_LIGHTS_PIN);
    int left = digitalRead(LEFT_TURN_PIN);
    int right = digitalRead(RIGHT_TURN_PIN);
  
    loopCounter = 0;
    isTurningLeft = left == 1 && right != 1;
    isTurningRight = left == 0 && right == 1;
    isStopping = left == 1 && right == 1;
    isRunningLights = runningLights == 1;

    if (isRunningLights != wasRunningLights) {
      Serial.print("RunningLights: ");
      Serial.println(isRunningLights);
      wasRunningLights = isRunningLights;
    }

    if (wasStopping != isStopping) {
      Serial.print("Stopping: ");
      Serial.println(isStopping);
      wasStopping = isStopping;
    }
    if (isTurningLeft != wasTurningLeft) {
      Serial.print("Turning Left: ");
      Serial.println(isTurningLeft);
      wasTurningLeft = isTurningLeft;
    }
    if (isTurningRight != wasTurningRight) {
      Serial.print("Turning Right: ");
      Serial.println(isTurningRight);
      wasTurningRight = isTurningRight;
    }
  }

  if (isStopping) {
    stop();
  } else if (isTurningLeft) {
    turnLeft();
  } else if (isTurningRight) {
    turnRight();
  }
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
  int offBrightness = wasRunningLights ? runningLightBrightness : 0;
  for (int x = -8; x < (NUM_COLUMNS / 2) + 8; x++) {
    if (x < 0 || x >= NUM_COLUMNS / 2) continue;
    // is column on?
    bool isOn = leftColIndex >= x && leftColIndex <= x + numOn;
    illuminateColumn(x, isOn ? fullBrightness : offBrightness, 0, 0);
  }
  FastLED.show();
  if (leftColIndex-- < -8){
    leftColIndex = (NUM_COLUMNS / 2) + 8;
  }
}

void turnRight() {
  // left to right
  int offBrightness = wasRunningLights ? runningLightBrightness : 0;
  for (int x = (NUM_COLUMNS / 2) - 8; x < NUM_COLUMNS + 8; x++) {
    if (x < NUM_COLUMNS / 2 || x > NUM_COLUMNS) continue;
    // is column on?
    bool isOn = rightColIndex <= x && rightColIndex >= x - numOn;
    illuminateColumn(x, isOn ? fullBrightness : offBrightness, 0, 0);
  }
  FastLED.show();
  if (rightColIndex++ > NUM_COLUMNS + 8){
    rightColIndex = (NUM_COLUMNS / 2) -8;
  }
}

void stop() {
  // right to left
  int offBrightness = wasRunningLights ? runningLightBrightness : 0;
  for (int x = 15; x >= 0; x--) {
    // is column on?
    bool isOn = stopColIndex <= (NUM_COLUMNS / 2) - x;
    illuminateColumn(x + 16, isOn ? fullBrightness : offBrightness, 0, 0);
    illuminateColumn(16 - x, isOn ? fullBrightness : offBrightness, 0, 0);
  }
  FastLED.show();
  if (stopColIndex > 0){
    stopColIndex--;
  }
}
