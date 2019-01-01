/*
  File: BraccioRobot.cpp

  Copyright 2018 Stefan Strömberg, stefangs@nethome.nu

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
  in compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied. See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "BraccioRobot.h"
#include <Arduino.h>

#define SOFT_START_CONTROL_PIN 12
#define BIG_JOINT_MAXIMUM_SPEED 140
#define DEFAULT_START_SPEED 40
#define MS_PER_S 1000
#define SOFT_START_TIME 1000

Position _BraccioRobot::initialPosition(90, 90, 90, 90, 90, 73);
_BraccioRobot BraccioRobot;

void _BraccioRobot::init(Position& startPosition, bool doSoftStart) {
  if (doSoftStart) {
    pinMode(SOFT_START_CONTROL_PIN,OUTPUT);
    digitalWrite(SOFT_START_CONTROL_PIN,LOW);
  }
  startSpeed = DEFAULT_START_SPEED;
  base.attach(11);
  shoulder.attach(10);
  elbow.attach(9);
  wristRotation.attach(5);
  wrist.attach(6);
  gripper.attach(3);
        
  base.write(startPosition.getBase());
  shoulder.write(startPosition.getShoulder());
  elbow.write(startPosition.getElbow());
  wrist.write(startPosition.getWrist());
  wristRotation.write(startPosition.getWristRotation());
  gripper.write(startPosition.getGripper());
  if (doSoftStart) {
    softStart();
  }
  currentPosition = startPosition;
}

void _BraccioRobot::softStart() {
  long int startTime=millis();
  while(millis()-startTime < SOFT_START_TIME) {
    digitalWrite(SOFT_START_CONTROL_PIN,LOW);
    delayMicroseconds(450);
    digitalWrite(SOFT_START_CONTROL_PIN,HIGH);
    delayMicroseconds(20); 
  }
}

void 
_BraccioRobot::powerOff() {
    digitalWrite(SOFT_START_CONTROL_PIN,LOW);
}

void 
_BraccioRobot::powerOn(){
    digitalWrite(SOFT_START_CONTROL_PIN,HIGH);
}


void 
_BraccioRobot::setStartSpeed(int speed){
  startSpeed = max(MINIMUM_SPEED, speed);
  startSpeed = min(MAXIMUM_SPEED, startSpeed);
}

void 
_BraccioRobot::moveToPosition(const Position& newPosition, int speed) {
  speed = max(MINIMUM_SPEED, speed);
  speed = min(MAXIMUM_SPEED, speed);
  int maxStepDelay = MS_PER_S/min(startSpeed, speed);
  int minStepDelay = MS_PER_S/speed;
  int stepDelay = maxStepDelay;
  int accellerationGap = maxStepDelay - minStepDelay;
  int accelleration = 1;
  double baseMove = currentPosition.getBase();
  double shoulderMove = currentPosition.getShoulder();
  double elbowMove = currentPosition.getElbow();
  double wristRotationMove = currentPosition.getWristRotation();
  double wristMove = currentPosition.getWrist();
  double gripperMove = currentPosition.getGripper();

  int maxMove = currentPosition.maxPositionDiff(newPosition);

  double baseStep = (newPosition.getBase() - currentPosition.getBase() + 0.0) / maxMove;
  double shoulderStep = (newPosition.getShoulder() - currentPosition.getShoulder() + 0.0) / maxMove;
  double elbowStep = (newPosition.getElbow() - currentPosition.getElbow() + 0.0) / maxMove;
  double wristRotationStep = (newPosition.getWristRotation() - currentPosition.getWristRotation() + 0.0) / maxMove;
  double wristStep = (newPosition.getWrist() - currentPosition.getWrist() + 0.0) / maxMove;
  double gripperStep = (newPosition.getGripper() - currentPosition.getGripper() + 0.0) / maxMove;

  /* Limit the speed of the big joints */
  if ((abs(baseStep) > 0.6) || (abs(shoulderStep) > 0.6)) {
    minStepDelay = max(MS_PER_S/BIG_JOINT_MAXIMUM_SPEED, minStepDelay);
  }

  int retardationPoint = maxMove / 2;
  if (maxMove > accellerationGap * 2) {
    retardationPoint = maxMove - accellerationGap;
  }

  for (int i = 0; i < maxMove; i++) {
    baseMove += baseStep;
    base.write(int(baseMove));
    shoulderMove += shoulderStep;
    shoulder.write(int(shoulderMove));
    elbowMove += elbowStep;
    elbow.write(int(elbowMove));
    wristRotationMove += wristRotationStep;
    wristRotation.write(int(wristRotationMove));
    wristMove += wristStep;
    wrist.write(int(wristMove));
    gripperMove += gripperStep;
    gripper.write(int(gripperMove));

    delay(stepDelay);

    if (i == retardationPoint) {
      accelleration = -accelleration;
    }
    stepDelay = max(minStepDelay, stepDelay - accelleration);
    stepDelay = min(maxStepDelay, stepDelay);
  }
  currentPosition = newPosition;
  base.write(currentPosition.getBase());
  shoulder.write(currentPosition.getShoulder());
  elbow.write(currentPosition.getElbow());
  wristRotation.write(currentPosition.getWristRotation());
  wrist.write(currentPosition.getWrist());
  gripper.write(currentPosition.getGripper());
}
