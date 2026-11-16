#include <Simple5641AS.h>
#include <Arduino.h>

// segmentPins[] = { a, b, c, d, e, f, g, dot };
const uint8_t segmentPins[] = { 4, 6, 3, 10, 9, 2, 5, 13 };
// digitSelectionPins[] = { D1, D2, D3, D4 };
const uint8_t digitSelectionPins[] = { 12, 11, 8, 7 };
Simple5641AS component( segmentPins, digitSelectionPins );
//
int leds[] = {41, 43, 45, 47, 49, 51, 53};
int numLeds = 7;
int potPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // countDown() + zeroAnimation() example
  Serial.println("countDown() + zeroAnimation() example");
  component.countDown(5);
  component.zeroAnimation();
  delay(1000);
  
  //
  // left -> right
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(getSpeed()); // read pot every step so knob feels live
    digitalWrite(leds[i], LOW);
  }
  // right -> left, skip ends for smooth bounce
  for (int i = numLeds - 2; i > 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(getSpeed());
    digitalWrite(leds[i], LOW);
  }
}

int getSpeed() {
  int potValue = analogRead(potPin); // 0 - 1023
  // 0 = fast, 1023 = slow. Flip to 500,30 if you want opposite.
  return map(potValue, 0, 1023, 30, 500);
}
