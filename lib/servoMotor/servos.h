#ifndef servo_h
#define servo_h

#include <Servo.h>
#include <Ramp.h>
#include <Arduino.h>
#include <math.h>

#define rightShoulderPin 3 //With the forearm servo (RS)
#define leftShoulderPin 5
#define forearmPin 6
#define clawPin 9

#define L1 116
#define L2 108.5

#define shoulderOffset 10
#define degreeAddition 30

//Intializes all of the servos
void initServos();

//Updates the servo movement and performs the movement
void updateServos();

//Moves the shoulder to an angle
void moveShoulder(int angleGoal, int motorTravelTime);

//Moves the forearm to an angle
void moveForearm(int angleGoal, int motorTravelTime);

//Opens the claw to an angke (0 degree means closed, 180 degrees means open)
void moveClaw(int angleGoal, int motorTravelTime);

//Animates the closing and opening of the claw
void animateClaw(int numOfTimes);

boolean doneMoving();

//Grabs the object
void grabObject();

//Drops the object
void dropObject();

//Implements IK algorithm to move the end effector to a XYZ position in space.
void moveTo(double Xd, double Yd);


#endif 