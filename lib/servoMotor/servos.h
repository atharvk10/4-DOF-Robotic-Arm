#ifndef servo_h
#define servo_h

#include <Servo.h>
#include <Ramp.h>
#include <Arduino.h>
#include <math.h>

constexpr int rightShoulderPin = 3;   // Right shoulder servo (shared axis with forearm)
constexpr int leftShoulderPin = 5;
constexpr int forearmPin = 6;
constexpr int clawPin = 9;

constexpr double L1 = 116.0;
constexpr double L2 = 108.5;

constexpr int lowAngleLimit = 0;
constexpr int highAngleLimit = 180;
const int shoulderOffset = 10;

/**
 * @brief Initializes the servos and their corresponding ramp objects.
 * @details Attaches each servo to its assigned pin, moves the servos to their
 * initial positions, and initializes the ramp objects used for smooth motion.
 */
void initServos();

/**
 * @brief Updates the servo positions.
 * @details Calls the ramp update functions and writes the resulting positions
 * to each servo. This function should be called repeatedly in the main loop.
 */
void updateServos();

/**
 * @brief Moves the shoulder to a desired angle.
 * @details The desired angle is measured with respect to the x-axis. The xy-plane
 * is defined as the plane passing through the center of the robot when viewed
 * from the side.
 * @param angleGoal Desired shoulder angle in degrees.
 * @param motorTravelTime Time, in milliseconds, for the servo to reach the
 * desired angle.
 */
void moveShoulder(int angleGoal, int motorTravelTime);

/**
 * @brief Moves the forearm to a desired angle.
 * @details Commands the forearm servo to move to the specified angle over the
 * given travel time.
 * @param angleGoal Desired forearm angle in degrees.
 * @param motorTravelTime Time, in milliseconds, for the servo to reach the
 * desired angle.
 */
void moveForearm(int angleGoal, int motorTravelTime);

/**
 * @brief Moves the claw to a desired angle.
 * @details Controls the opening and closing of the claw. An angle of 0° represents
 * a fully closed claw, while 180° represents a fully open claw.
 * @param angleGoal Desired claw angle in degrees.
 * @param motorTravelTime Time, in milliseconds, for the servo to reach the
 * desired angle.
 */
void moveClaw(int angleGoal, int motorTravelTime);

/** 
 * @brief Animates the claw by repeatedly opening and closing it.
 * @details Used to demonstrate or test the claw's operation.
 * @param numOfTimes Number of times the claw should open and close.
 */
void animateClaw(int numOfTimes);

/**
 * @brief Checks whether the arm has completed all commanded movements.
 * @return True if both the shoulder and forearm have reached their target
 * positions; otherwise, false.
 */
bool armDoneMoving();

/**
 * @brief Checks whether the claw has completed its commanded movement.
 * @return True if the claw has reached its target position; otherwise, false.
 */
bool clawDoneMoving();

/**
 * @brief Closes the claw to grasp an object.
 * @details Moves the claw to a predefined closed position suitable for gripping
 * a cube.
 */
void grabCube();

/**
 * @brief Opens the claw to release an object.
 * @details Moves the claw to a predefined open position to release a held cube.
 */
void dropCube();

/**
 * @brief Moves the end effector to a desired position using inverse kinematics.
 * @details Calculates the required shoulder and forearm joint angles from the
 * specified Cartesian coordinates and commands the servos to those positions.
 * @param Xd Desired x-coordinate of the end effector in millimeters.
 * @param Yd Desired y-coordinate of the end effector in millimeters.
 */
void moveTo(double Xd, double Yd);

#endif