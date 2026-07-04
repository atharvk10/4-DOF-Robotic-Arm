#ifndef StepMotor_h
#define StepMotor_h

#include <AccelStepper.h>
#include <Arduino.h>

// Pin definitions for the stepper motor driver
#define STEP_PIN 12
#define DIR_PIN 11
#define ENABLE_PIN A3
#define MS1 A2
#define MS2 A1

// Stepper motor settings
#define stepRes 16
#define gearRatio 2.5
#define stepsPerRevolution 200 * stepRes * gearRatio
#define maxSpeed 2000
#define accelVal 2000

/**
 * @brief Initializes the stepper motor.
 * @details Configures the stepper motor driver, sets the maximum speed and
 * acceleration, and enables the motor for operation. This function should be
 * called once during setup.
 */
void setupStepper();

/**
 * @brief Rotates the stepper motor by a specified percentage of a full revolution.
 * @details Commands the stepper motor to rotate a given percentage of one full
 * revolution in either the clockwise or counterclockwise direction.
 * @param percentage Percentage of one full revolution to rotate.
 * @param dir Rotation direction. Valid values are "CW" for clockwise and
 * "CCW" for counterclockwise.
 */
void rotate(double percentage, String dir);

/**
 * @brief Updates the stepper motor position.
 * @details Advances the stepper motor toward its target position according to
 * the configured acceleration and speed profile. This function should be
 * called repeatedly in the main loop.
 */
void updateStepper();

/**
 * @brief Checks whether the stepper motor has completed its commanded movement.
 * @return True if the stepper motor has reached its target position;
 * otherwise, false.
 */
boolean stepperDone();

#endif