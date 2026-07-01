#include <Arduino.h>
#include "../lib/servoMotor/servos.h"
#include "../lib/stepperMotor/stepperMotor.h"
#include "../lib/breakBeam/breakbeam.h"

boolean objectDetected = false;
boolean homedRobot = false;

//Updated Homing Positions
const int leftShoulderFinal = 90;
const int rightShoulderFinal = 180 - leftShoulderFinal + shoulderOffset;
const int forearmFinal = 30;
const int clawFinal = 135;

//State Machine Variables

enum RobotState {
  HOME,
  WAITING, 
  PICKING_UP,
  RETURNING_HOME,
  ROTATING
};


void setup() {

  Serial.begin(9600);

  setupStepper();
  setupBreakBeam();
  initServos();

  delay(3000);

}

void loop() {

  updateServos();

  if (!homedRobot) {
      moveShoulder(leftShoulderFinal, 1000);
      moveForearm(forearmFinal, 1000);
      moveClaw(clawFinal, 1000);
      homedRobot = true;

      Serial.println("Servo Setup and Home ✅");
      Serial.println("\nFull Setup Complete! ✅");

  }

  if(doneMoving()) return;

  objectDetected = checkObject();

  if(objectDetected) {
    
    Serial.println("Object Detected!");
    delay(1000);

    //---- GRABBING OBJECT ----
    //rotate(0.25, "CW"); //Rotate towards the cube
    moveShoulder(32, 1500);
    moveForearm(0, 1500);    
    grabObject(); //Grab object
    Serial.println("Object Picked Up!");

    // ---- NEUTRAL POSITION BEFORE DROPPING
    delay(1000);
    moveShoulder(90, 1000);
    moveForearm(30, 1000);    // delay(1000);
    //if(doneMoving()) rotate(0.5, "CW");



    // // -- DROPPING OBJECT
    // rotate(0.25, "CCW");
    // delay(500);
    // moveShoulder(32, 1500);
    // moveForearm(0, 1500);    
    // dropObject();

    // Serial.println("Object Dropped!");

    objectDetected = false;

  }

  

}
