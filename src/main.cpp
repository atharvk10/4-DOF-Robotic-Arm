/**
 * @author Atharv Koratkar
 * @date July 2026
 */

#include <Arduino.h>
#include "../lib/servoMotor/servos.h"
#include "../lib/stepperMotor/stepperMotor.h"
#include "../lib/breakBeam/breakbeam.h"

boolean objectDetected = false;
boolean homedRobot = false;

//Final homing positions
const int leftShoulderFinal = 90;
const int rightShoulderFinal = 180 - leftShoulderFinal + shoulderOffset;
const int forearmFinal = 30;
const int clawFinal = 25;

//Positions for grabbing the cube
const int shoulderGrabPos = 32;
const int forearmGrabPos = 0;

//Positions for placing the cube
const int shoulderPlacePos = 45;
const int forearmPlacePos = 0;

//Constants for stepper motor movement
const double rotateToCube = 0.125;
const double rotateToBox = 0.375;
const double rotateToHome = 0.25;
const double rotateToMiddle = 0.125;

//Constants for servo movement
const int servoTime = 750;
const int timeElapsed = 1000;
const int settleTime = 1000;

long randNum;
long currentRandValue;
int upperLimit = 51;

//State Machine Variables
enum RobotState {
  HOME,
  WAITING,
  ROTATE_TO_CUBE,
  MOVING_TO_CUBE,
  GRABBING_CUBE,
  MOVING_TO_NEUTRAL,
  ROTATING_TO_BOX,
  DROPPING_CUBE,
  MOVING_TO_BOX,
  ROTATING_TO_HOME
};

RobotState ROBOSTATE = HOME;
boolean stateEntered = false;
boolean rotationStarted = false;
long startTime = millis();

void setup() {

  Serial.begin(9600);
  setupStepper();
  setupBreakBeam();
  initServos();

  delay(settleTime);

  randomSeed(analogRead(A0));

}

void loop() {

  updateServos();
  updateStepper();

  randNum = random(upperLimit);

  switch(ROBOSTATE) {

    case HOME: //MOVING TO THE HOME POSITION 

      if(!stateEntered) {
        moveShoulder(leftShoulderFinal, servoTime);
        moveForearm(forearmFinal, servoTime);
        moveClaw(clawFinal, servoTime);

        Serial.println("----------------------");
        Serial.println("\n1. ROBOT HOMED");
        Serial.println("\n2. WAITING FOR CUBE");
        stateEntered = true;
      }

      if(armDoneMoving()) {
        stateEntered = false;
        ROBOSTATE = WAITING;
      }

      break;
    
    case WAITING: //CHECKING IF A CUBE IS DETECTED
      
      objectDetected = checkCube();

      if(objectDetected) {
        animateClaw(1);
        ROBOSTATE = ROTATE_TO_CUBE;
      }

      break;

    case ROTATE_TO_CUBE: //ROTATING TOWARDS THE CUBE

      if (!stateEntered) {
        rotate(rotateToCube, "CW");
        stateEntered = true;
      }

      if (stepperDone()) {
        stateEntered = false;
        ROBOSTATE = MOVING_TO_CUBE;
      }

      break;

    case MOVING_TO_CUBE: //MOVING ARM TO GRAB THE CUBE

      if(!stateEntered) {
        Serial.println("\n3. MOVING TO CUBE");
        moveShoulder(shoulderGrabPos, servoTime);
        moveForearm(forearmGrabPos, servoTime); 

        stateEntered = true;
      } 

      if(armDoneMoving()) {
        stateEntered = false;
        startTime = millis();
        ROBOSTATE = GRABBING_CUBE;
      }

      break;

    case GRABBING_CUBE: //GRABBING CUBE

      if(millis() - startTime > timeElapsed) {
        grabCube(); 
        Serial.println("\n4. GRABBING CUBE");
        ROBOSTATE = MOVING_TO_NEUTRAL;
      }

      break;

    case MOVING_TO_NEUTRAL: //MOVING TO THE CENTRAL POSITION

      if(!stateEntered) {
        Serial.println("\n5. MOVING TO NEUTRAL POSITION");
        moveShoulder(leftShoulderFinal, servoTime);
        moveForearm(forearmFinal, servoTime);
        stateEntered = true;
      }
      
      if(armDoneMoving()) {
        stateEntered = false;
        ROBOSTATE = ROTATING_TO_BOX;
      }

      break;
        
    case ROTATING_TO_BOX: //ROTATING TO THE BOX TO PLACE CUBE

      if (!stateEntered) {
        startTime = millis();
        stateEntered = true;
      }

      if (!rotationStarted && millis() - startTime >= timeElapsed) {
        if(randNum % 2 == 0) rotate(rotateToBox, "CCW"); //Rotate to the box
        else rotate(rotateToMiddle, "CCW"); //Rotate to the middle box

        currentRandValue = randNum;
        rotationStarted = true;
      }

      if (rotationStarted && stepperDone()) {
        rotationStarted = false;
        stateEntered = false;
        ROBOSTATE = MOVING_TO_BOX;
      }

      break;
    
    case MOVING_TO_BOX: //MOVING ARM TO PLACE CUBE
      
      if(!stateEntered) {
        Serial.println("\n7. MOVING ARM TO BOX");
        moveShoulder(shoulderPlacePos, servoTime);
        moveForearm(forearmGrabPos, servoTime);

        stateEntered = true;
      }

      if(armDoneMoving()) {
        stateEntered = false;
        startTime = millis();
        ROBOSTATE = DROPPING_CUBE;
      }

      break;

    case DROPPING_CUBE: //DROPPING THE CUBE IN THE BOX

      if(millis() - startTime > timeElapsed) {
        Serial.println("\n8. DROPPING CUBE IN BOX");
        dropCube(); 
        if(currentRandValue % 2 == 0) ROBOSTATE = ROTATING_TO_HOME;
        else ROBOSTATE = HOME;
      }

      break;
    
    case ROTATING_TO_HOME: //ROTATING TOWARDS THE HOME POSITION
      
      if(!stateEntered) {
        Serial.println("\n9. RETURNING HOME");
        rotate(rotateToHome, "CW");

        stateEntered = true;
      } 

      if(stepperDone()) {
        stateEntered = false;
        ROBOSTATE = HOME;
      }

      break;


  }
}  