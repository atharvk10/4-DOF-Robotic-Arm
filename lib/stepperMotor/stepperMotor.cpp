#include "stepperMotor.h"

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setupStepper() {

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);

  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);

  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(accelVal);
  stepper.setEnablePin(ENABLE_PIN);

  stepper.setPinsInverted(false, false, true); 
  stepper.enableOutputs();

  stepper.setCurrentPosition(0);
  stepper.moveTo(0);

  Serial.println("Stepper Setup ✅");
}

void rotate(double percentage, String dir) {

  stepper.enableOutputs();
  int target = (int)(percentage * stepsPerRevolution);

  if(dir.equalsIgnoreCase("CCW")) {
    target = -target;
  } 

  stepper.move(target);

  while(stepper.distanceToGo() != 0) {
     stepper.run();
  }

  stepper.stop();
  stepper.disableOutputs();

  
}