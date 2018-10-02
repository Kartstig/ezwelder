#include "sensor.h"

#define BOOT 0
#define WAIT 1
#define WELD 2

const byte ledPin = 13;
const byte relayPin1 = 10;
const byte controlPin = A0;
const byte buttonPin = A1;
const unsigned long debounce = 950;
const char splashText[16] = "Spot Welder 1.0";
const int durationLOW = 5;
const int durationHIGH = 201;

volatile unsigned long startTime;
volatile byte buttonState = HIGH;
volatile unsigned long lastPress = 0;
volatile unsigned int count = 0;
volatile int duration = 80;
volatile int state = 0;

void clearScreen() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x01); // Clear Display
}

void setBrightness() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x80); // Brightness
}

int getDuration() {
  int raw_value = averageRead(controlPin);
  float duty = (float) raw_value / 1024;
  int offset = (int) (duty*(durationHIGH-durationLOW));
  return durationLOW + offset;
}

void render() {
  clearScreen();
  const char row1[16];
  const char row2[16];
  sprintf(row1, "Duration: %i ms ", duration);
  Serial.write(0xFE);
  Serial.write(0x80);
  Serial.write(row1);
  sprintf(row2, "Count:    %i    ", count);
  Serial.write(0xFE);
  Serial.write(0xC0);
  Serial.write(row2);
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin1, LOW);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  while(!Serial) {}; // Wait for serial to begin
  delay(500);
  setBrightness();
  clearScreen();
}

void loop()
{
  switch (state)
  {
    case BOOT:
      duration = getDuration();
      render();
      state = WAIT;
    case WAIT:
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW && ((millis()-lastPress) > debounce))
      {
        digitalWrite(ledPin, HIGH);  // turn on the LED
        digitalWrite(relayPin1, HIGH);  // turn on relay1
        startTime = millis();
        state = WELD;
      } else {
        duration = getDuration();
        render();
      }
      break;

    case WELD:
      if (millis() - startTime >= duration)  // Debounce the button press
      {
        digitalWrite(relayPin1, LOW);  // turn off relay 1
        digitalWrite(ledPin, LOW);  // turn on the LED
        lastPress = millis();
        count++;
        state = WAIT;
      }
      break;
  }
}
