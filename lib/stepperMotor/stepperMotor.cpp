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

  Serial.println("Stepper Motor Setup ✅");
}

void rotate(double percentage, String dir)
{
    stepper.enableOutputs();

    long target = (long)(percentage * stepsPerRevolution);

    if (dir.equalsIgnoreCase("CCW"))
        target = -target;

    stepper.move(target);   // Start moving only
}

void updateStepper()
{
    if (stepper.distanceToGo() != 0)
    {
        stepper.run();
    }
    else
    {
        stepper.disableOutputs();
    }
}

boolean stepperDone()
{
    return stepper.distanceToGo() == 0;
}