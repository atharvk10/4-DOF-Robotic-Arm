#ifndef StepMotor_h
#define StepMotor_h

#include <AccelStepper.h>
#include <Arduino.h>

//Pin definition for movement
#define STEP_PIN 12
#define DIR_PIN 11
#define ENABLE_PIN A3
#define MS1 A2
#define MS2 A1

//Stepper motor settings
#define stepRes 16
#define gearRatio 2.5
#define stepsPerRevolution 200 * stepRes * gearRatio
#define maxSpeed 2000
#define accelVal 2000

//Setting the stepper motor
void setupStepper();

//Percentage is % of full revolution we want to rotate, dir is either "CCW" or "CW"
void rotate(double percentage, String dir);

void updateStepper();

boolean stepperDone();





#endif