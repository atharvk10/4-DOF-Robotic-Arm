#include <Arduino.h>
#include "../lib/servoMotor/servos.h"
#include "../lib/stepperMotor/stepperMotor.h"
#include "../lib/breakBeam/breakbeam.h"

boolean objectDetected = false;

void setup() {

  Serial.begin(9600);

  setupStepper();
  setupBreakBeam();
  initServos();
  updateServos();

  delay(2000);

  Serial.println("\nFull Setup Complete! ✅");

}

void loop() {

  objectDetected = checkObject();

  if(objectDetected) {
    Serial.println("Object Detected!");
    delay(2000);

    //---- GRABBING OBJECT ----
    rotate(0.25, "CW"); //Rotate towards the cube
    delay(500);
    moveTo(10, 5); //Need to figure out the x and y position to pick up the object
    delay(1000);
    grabObject(); //Grab object
    Serial.println("Object Picked Up!");

    // ---- NEUTRAL POSITION BEFORE DROPPING
    delay(1000);
    moveTo(10, 5); //Need to figure out the x and y position to hold the cube in between the two places
    delay(500);
    rotate(0.25, "CCW");

    // -- DROPPING OBJECT
    delay(1000);
    rotate(0.25, "CCW");
    delay(500);
    moveTo(10, 5);
    delay(500);
    dropObject();

    Serial.println("Object Dropped!");

    objectDetected = false;

  }

  updateServos();

  

}
