#include "breakbeam.h"

int lastState;

void setupBreakBeam() {

  pinMode(sensorPin, INPUT_PULLUP);
  lastState = digitalRead(sensorPin);
  
  Serial.println("Started");

}

boolean checkObject() {
  int currentState = digitalRead(sensorPin);

  if (currentState != lastState) {
    if (currentState == LOW) {
      Serial.println("Object Detected!");
      return true;
    } else {
      Serial.println("Object Cleared!");
      return false;
    }
    lastState = currentState;
  }

}