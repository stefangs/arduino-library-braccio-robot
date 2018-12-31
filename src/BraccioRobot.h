/*
  File: BraccioRobot.h

  Copyright 2018 Stefan Strömberg, stefangs@nethome.nu
  
  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
  in compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied. See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef BRACCIO_ROBOT_H_
#define BRACCIO_ROBOT_H_

#include <Servo.h>
#ifndef Servo_h
#error Sketch must include Servo.h
#endif

#include "Position.h"

#define MAXIMUM_SPEED 200
#define MINIMUM_SPEED 20

class _BraccioRobot {

public:
  static Position initialPosition;
  void init(Position& startPosition=_BraccioRobot::initialPosition, bool doSoftStart=true);

  /* Moves the arm to the specified position with the specified speed. The movement will start at startSpeed and accelerate
     towards specified speed. At the end of the movement it will decelerate back to startSpeed before reaching
     end position. Speeds are adjusted so that all joints will start and finish at the same time regardless
     of varying travel distance. Speed is in degrees/second and can be between 20 and 200 */
  void moveToPosition(const Position& newPosition, int speed);

  /* Turns off power to the servo motors. This only work if you are using a robot shield later than V1.6.
     Note that after a call to init() the power is on */
  void powerOff();

  /* Turns on power to the servo motors. This only work if you are using a robot shield later than V1.6 */
  void powerOn();

  /* Set start speed of movement in degrees/second. Between 20 and 200 */
  void setStartSpeed(int speed);
  inline int getStartSpeed() const {return startSpeed;}


private:
  Servo base;
  Servo shoulder;
  Servo elbow;
  Servo wristRotation;
  Servo wrist;
  Servo gripper;
  Position currentPosition;
  int startSpeed;
  
  void softStart();
};

extern _BraccioRobot BraccioRobot;

#endif // BRACCIO_ROBOT_H_
