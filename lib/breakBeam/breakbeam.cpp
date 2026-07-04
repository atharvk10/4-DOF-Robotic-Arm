/**
 * @author Atharv Koratkar
 * @date July 2026
 */

#include "breakbeam.h"

// 1 = beam clear, 0 = object present
int lastState = 1;

void setupBreakBeam()
{  
    //Initializes the sensor pin and stores initial state of break beam.
    pinMode(sensorPin, INPUT);
    lastState = digitalRead(sensorPin);
    
    Serial.println("Break Beam Setup! ✅");
}

bool checkCube()
{   
    //Checks current state of break beam.
    int currentState = digitalRead(sensorPin);
    bool cubeDetected = false;

    //Detecting a change of state from empty to cube detected.
    if (lastState == 1 && currentState == 0)
    {
        Serial.println("\tCube Detected!");
        cubeDetected = true;
    }

    //Detecting if the cube has been cleared from the break beam.
    else if (lastState == 0 && currentState == 1) Serial.println("\tCube Cleared!");

    //Storing the most recent state before running again.
    lastState = currentState;

    return cubeDetected;
}