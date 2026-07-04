/**
 * @author Atharv Koratkar
 * @date July 2026
 */

#include "servos.h"

//Declaring servo and ramp objects
Servo leftBase, rightBase, forearm, claw;
rampInt shoulderRamp;
rampInt forearmRamp;
rampInt clawRamp;

//Initial positions upon startup
const int leftShoulderInitial = 90;
const int rightShoulderInitial = 180 - leftShoulderInitial + shoulderOffset;
const int forearmInitial = 90;
const int clawInitial = 25;

void initServos()
{
    // Attach servos
    leftBase.attach(leftShoulderPin);
    rightBase.attach(rightShoulderPin);
    forearm.attach(forearmPin);
    claw.attach(clawPin);

    // Move servos to a known physical position
    leftBase.write(leftShoulderInitial);
    rightBase.write(rightShoulderInitial);
    forearm.write(180 - forearmInitial);
    claw.write(clawInitial);

    // Synchronize Ramp with the actual servo positions
    shoulderRamp.go(leftShoulderInitial, 0);
    forearmRamp.go(180 - forearmInitial, 0);
    clawRamp.go(clawInitial, 0);

    Serial.println("Servo Setup ✅");

}

void updateServos()
{
    int shoulderPos = shoulderRamp.update();

    leftBase.write(shoulderPos);
    rightBase.write(180 - shoulderPos);

    forearm.write(forearmRamp.update());
    claw.write(clawRamp.update());
}

void moveShoulder(int angleGoal, int motorTravelTime)
{
    angleGoal = constrain(angleGoal, lowAngleLimit, highAngleLimit);
    shoulderRamp.go(180 - angleGoal, motorTravelTime);
}

void moveForearm(int angleGoal, int motorTravelTime)
{
    angleGoal = constrain(angleGoal, lowAngleLimit, highAngleLimit);
    forearmRamp.go(180 - angleGoal, motorTravelTime);
}

void moveClaw(int angleGoal, int motorTravelTime)
{
    angleGoal = constrain(angleGoal, lowAngleLimit, highAngleLimit);
    clawRamp.go(angleGoal, motorTravelTime);
}

void animateClaw(int numOfTimes) {

    claw.write(25);

    for(int i = 25; i < numOfTimes; i++) {
        for(int j = 0; j < 170; j++) {
            claw.write(j);
            delay(5);
        }

        for(int k = 170; k > 25; k--) {
            claw.write(k);
            delay(5);
        }
    }
    
}

boolean armDoneMoving() {

    return !shoulderRamp.isFinished() || !forearmRamp.isFinished();
}

boolean clawDoneMoving() {
    
    return !clawRamp.isFinished();
}

void grabCube()
{

    moveClaw(25, 500);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(90, 500);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(165, 500);
    while (!clawRamp.isFinished())
        updateServos();
}

void dropCube()
{
    moveClaw(165, 500);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(90, 500);
    while (!clawRamp.isFinished())
        updateServos();
        
}

void moveTo(double Xd, double Yd)
{
    // Convert cm to mm
    Xd *= 10.0;
    Yd *= 10.0;

    double distance = sqrt(Xd * Xd + Yd * Yd);

    if (distance > (L1 + L2))
    {
        Serial.println("Target out of reach.");
        return;
    }

    double forearm_angle =
        PI - acos(((Xd * Xd + Yd * Yd) - (L1 * L1 + L2 * L2)) /
                  (-2.0 * L1 * L2));

    double alpha = atan2(Yd, Xd);

    double beta =
        atan2(L2 * sin(forearm_angle),
              L1 + L2 * cos(forearm_angle));

    double shoulder_angle = alpha + beta;

    double shoulderDeg = shoulder_angle * 180.0 / PI;
    double forearmDeg = 270.0 - forearm_angle * 180.0 / PI;

    Serial.print("Shoulder: ");
    Serial.println(shoulderDeg);

    Serial.print("Forearm: ");
    Serial.println(forearmDeg);

  //  moveShoulder((int)shoulderDeg);
   // moveForearm((int)forearmDeg);
}