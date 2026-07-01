#include "breakbeam.h"

// 1 = beam clear, 0 = object present
int lastState = 1;

void setupBreakBeam()
{
    pinMode(sensorPin, INPUT);
    lastState = digitalRead(sensorPin);

    Serial.println("Break Beam Setup! ✅");
}

bool checkObject()
{
    int currentState = digitalRead(sensorPin);

    bool detected = false;

    // ONLY trigger on transition HIGH → LOW
    if (lastState == 1 && currentState == 0)
    {
        //Serial.println("Object Detected!");
        detected = true;
    }

    // ONLY trigger on transition LOW → HIGH
    else if (lastState == 0 && currentState == 1)
    {
     //   Serial.println("Object Cleared!");
    }

    // ALWAYS update last state
    lastState = currentState;

    return detected;
}