#include "servos.h"

Servo leftBase, rightBase, forearm, claw;

rampInt shoulderRamp;
rampInt forearmRamp;
rampInt clawRamp;

const int leftShoulderInitial = 135;
const int rightShoulderInitial = 180 - leftShoulderInitial + shoulderOffset;

const int forearmInitial = 30;
const int clawInitial = 45;


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

    delay(500);

    // Synchronize Ramp with the actual servo positions
    shoulderRamp.go(leftShoulderInitial, 0);
    forearmRamp.go(180 - forearmInitial, 0);
    clawRamp.go(clawInitial, 0);

    Serial.println("Servo Setup and Home ✅");
}

void updateServos()
{
    int shoulderPos = shoulderRamp.update();

    leftBase.write(shoulderPos);
    rightBase.write(180 - shoulderPos);

    forearm.write(forearmRamp.update());
    claw.write(clawRamp.update());
}

void moveShoulder(int angleGoal)
{
    shoulderRamp.go(angleGoal, motorTimeTravel);
}

void moveForearm(int angleGoal)
{
    forearmRamp.go(angleGoal, motorTimeTravel);
}

void moveClaw(int angleGoal)
{
    clawRamp.go(angleGoal, motorTimeTravel);
}

void grabObject()
{
    Serial.println("Grabbing object!");

    moveClaw(45);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(90);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(180);
    while (!clawRamp.isFinished())
        updateServos();
}

void dropObject()
{
    moveClaw(0);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(90);
    while (!clawRamp.isFinished())
        updateServos();

    moveClaw(180);
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

    moveShoulder((int)shoulderDeg);
    moveForearm((int)forearmDeg);
}