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
const int clawFinal = 90;

//Positions for grabbing the cube
const int shoulderGrabPos = 32;
const int forearmGrabPos = 0;

//Constants for stepper motor movement
const double rotateTo = 0.25;
const double rotateBack = 0.5;




//State Machine Variables
enum RobotState {
  HOME,
  WAITING,
  ROTATE_TO_CUBE,
  MOVING_TO_CUBE,
  GRABBING_CUBE,
  MOVING_TO_CENTRAL,
  ROTATING_TO_BOX,
  DROPPING_CUBE,
  MOVING_TO_BOX,
  ROTATING_TO_HOME
};

RobotState ROBOSTATE = HOME;
boolean stateEntered = false;
long startTime = millis();

void setup() {

  Serial.begin(9600);
  setupStepper();
  setupBreakBeam();
  initServos();

  delay(1000);

}

void loop() {

  updateServos();
  updateStepper();

  switch(ROBOSTATE) {

    case HOME: //MOVING TO THE HOME POSITION 

      if(!stateEntered) {
        moveShoulder(leftShoulderFinal, 1000);
        moveForearm(forearmFinal, 1000);
        moveClaw(clawFinal, 1000);

        Serial.println("\n1. ROBOT HOMED");
        stateEntered = true;
      }

      if(armDoneMoving()) {
        stateEntered = false;
        ROBOSTATE = WAITING;
      }

      break;
    
    case WAITING: //CHECKING IF A CUBE IS DETECTED
      
      objectDetected = checkObject();
      if(objectDetected) ROBOSTATE = ROTATE_TO_CUBE;
      break;

    case ROTATE_TO_CUBE: //ROTATING TOWARDS THE CUBE

      if (!stateEntered)
      {
        rotate(rotateTo, "CW");
        stateEntered = true;
      }

      if (stepperDone())
      {
        stateEntered = false;
        ROBOSTATE = MOVING_TO_CUBE;
      }

      break;

    case MOVING_TO_CUBE: //MOVING ARM TO GRAB THE CUBE

      if(!stateEntered) {
        Serial.println("\n3. MOVING TO CUBE");
        moveShoulder(shoulderGrabPos, 1000);
        moveForearm(forearmGrabPos, 1000); 

        stateEntered = true;
      } 

      if(armDoneMoving()) {
        stateEntered = false;
        startTime = millis();
        ROBOSTATE = GRABBING_CUBE;
      }

      break;

    case GRABBING_CUBE: //GRABBING CUBE

      if(millis() - startTime > 2000) {
        Serial.println("\n4. GRABBING CUBE");
        grabCube(); 
        ROBOSTATE = MOVING_TO_CENTRAL;
      }
      break;

    case MOVING_TO_CENTRAL:

      if(!stateEntered) {
        Serial.println("\n5. MOVING TO CENTRAL POSITION");
        moveShoulder(90, 1500);
        moveForearm(30, 1500);
        stateEntered = true;
      }
      
      if(armDoneMoving()) {
        stateEntered = false;
        ROBOSTATE = ROTATING_TO_BOX;
      }

      break;
        
    case ROTATING_TO_BOX:
      
      if (!stateEntered)
      {
        Serial.println("\n6. ROTATING TO BOX");
        rotate(rotateBack, "CCW");
        delay(500);
        stateEntered = true;
      }

      if (stepperDone())
      {
        stateEntered = false;
        ROBOSTATE = MOVING_TO_BOX;
      }
    
      break;
    
    case MOVING_TO_BOX:
      
      if(!stateEntered) {
        Serial.println("\n7. MOVING ARM TO BOX");
        moveShoulder(45, 1500);
        moveForearm(0, 1500);

        stateEntered = true;
      }

      if(armDoneMoving()) {
        stateEntered = false;
        startTime = millis();
        ROBOSTATE = DROPPING_CUBE;
      }

      break;

    case DROPPING_CUBE:

        Serial.println("\n8. DROPPING CUBE IN BOX");
        if(millis() - startTime > 2000) {
          dropCube(); 
          ROBOSTATE = ROTATING_TO_HOME;
        }

      break;
    
    case ROTATING_TO_HOME:
      
      if(!stateEntered) {
        Serial.println("\n9. RETURNING HOME");
        rotate(0.25, "CW");

        stateEntered = true;
      } 

      if(stepperDone()) {
        stateEntered = false;
        ROBOSTATE = HOME;
      }

      break;


  }
}  