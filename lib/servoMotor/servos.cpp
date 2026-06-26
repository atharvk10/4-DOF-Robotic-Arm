#include "servos.h"

Servo leftBase, rightBase, forearm, claw;
rampInt shoulderRamp, forearmRamp, clawRamp;

//Initial Positions
int leftShoulderInitial = 90, 
    rightShoulderInitial = leftShoulderInitial + shoulderOffset, 
    forearmInitial = 135, clawInitial = 45; //180 is closed

void initServos() {

    leftBase.write(leftShoulderInitial);
    rightBase.write(rightShoulderInitial);
    forearm.write(forearmInitial);
    claw.write(clawInitial);

    leftBase.attach(leftShoulderPin, 500, 2500);
    rightBase.attach(rightShoulderPin, 500, 2500);
    forearm.attach(forearmPin, 500, 2500);
    claw.attach(clawPin, 500, 2500);

    initialPositions();
    updateServos();

}

void initialPositions() {

    shoulderRamp.go(leftShoulderInitial, motorTimeTravel);
    forearmRamp.go(180 - forearmInitial, motorTimeTravel);
    clawRamp.go(clawInitial, motorTimeTravel);

    Serial.println("Servo Setup and Home ✅");

}

void updateServos() {

    int shoulderPos = shoulderRamp.update();
    leftBase.write(shoulderPos);
    rightBase.write(180 - shoulderPos);

    forearm.write(forearmRamp.update());
    claw.write(clawRamp.update());
  
}

void moveShoulder(int angleGoal) {shoulderRamp.go(angleGoal, motorTimeTravel);}
void moveForearm(int angleGoal) {forearmRamp.go(angleGoal, motorTimeTravel);}
void moveClaw(int angleGoal) {clawRamp.go(angleGoal, motorTimeTravel);}

void grabObject() {

  Serial.println("Grabbing object!");
  moveClaw(45);
  delay(100);
  moveClaw(90);
  delay(100);
  moveClaw(180);

}

void dropObject() {

    moveClaw(0);
    delay(100);
    moveClaw(90);
    delay(100);
    moveClaw(180);
}


void moveTo(double Xd, double Yd)
{   

    //Converting coordinates to mm
    Xd = Xd * 10; 
    Yd = Yd * 10;

    //Checking if target is beyond what link lengths
    if(Xd > L1 + L2) return;

    //Solving for forearm angle
    double forearm_angle = PI - acos(((Xd*Xd + Yd*Yd) - (L1*L1 + L2*L2)) / (-2 * L1 * L2)); //acos returns radians

    //Solving for the shoulder angle
    double alpha = atan2(Yd, Xd); //Angle between horizontal and elbow up position
    double beta = atan2(L2 * sin(forearm_angle), L1 + L2 * cos(forearm_angle)); //Angle between both IK solutions
    double shoulder_angle = alpha + beta;

    //Converting the radians to degrees
    double shoulder_angle_degrees = shoulder_angle * (180/PI);
    double forearm_angle_degrees = 270 - (forearm_angle * (180/PI));

    //Printing out the calculated angles
    Serial.print("Shoulder Angle: ");
    Serial.println(shoulder_angle_degrees);
    Serial.print("Forearm Angle: ");
    Serial.println(forearm_angle_degrees);

    //Moving the shoulder and forearm
    moveShoulder(shoulder_angle_degrees);
    moveForearm(forearm_angle_degrees);
    updateServos();

}