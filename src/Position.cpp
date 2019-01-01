/*
  File: Position.cpp

  Copyright 2018 Stefan Strömberg, stefangs@nethome.nu

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
  in compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied. See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "Position.h"
#include "Arduino.h"

Position&
Position::setBase(int b) {
  base = limit(b, 0, 180);
  return *this;
}

Position&
Position::setShoulder(int s) {
  shoulder = limit(s, 15, 165);
  return *this;
}

Position&
Position::setElbow(int e){
  elbow = limit(e, 0, 180);
  return *this;
}

Position& 
Position::setWristRotation(int w){
  wristRotation = limit(w, 0, 180);
  return *this;
}

Position&
Position::setWrist(int v){
  wrist = limit(v, 0, 180);
  return *this;
}

Position&
Position::setGripper(int g){
  gripper = limit(g, 10, 73);
  return *this;
}

int
Position::limit(int value, int minv, int maxv) {
  int result;
  if (value < minv) {
    result = minv;
  } else if (value > maxv) {
    result = maxv;
  } else {
    result = value;
  }
  return result;
}

Position::Position() {
  Position(90, 90, 90, 90, 90, 73);
}

Position::Position(int basePos, int shoulderPos, int elbowPos, int wristRotationPos, int wristPos, int gripperPos) {
  set(basePos, shoulderPos, elbowPos, wristRotationPos, wristPos, gripperPos);
}

Position&
Position::set(int basePos, int shoulderPos, int elbowPos, int wristPos, int wristRotationPos, int gripperPos) {
  setBase(basePos);
  setShoulder(shoulderPos);
  setElbow(elbowPos);
  setWrist(wristPos);
  setWristRotation(wristRotationPos);
  setGripper(gripperPos);
  return *this;
}

Position& 
Position::operator=(const Position& p) {
  base = p.base;
  shoulder = p.shoulder;
  elbow = p.elbow;
  wristRotation = p.wristRotation;
  wrist = p.wrist;
  gripper = p.gripper;
  return *this;
}

int 
Position::maxPositionDiff(const Position& p) const {
  int maxDiff = 0;
  maxDiff = max(maxDiff, abs(base - p.base));
  maxDiff = max(maxDiff, abs(shoulder - p.shoulder));
  maxDiff = max(maxDiff, abs(elbow - p.elbow));
  maxDiff = max(maxDiff, abs(wrist - p.wrist));
  maxDiff = max(maxDiff, abs(wristRotation - p.wristRotation));
  maxDiff = max(maxDiff, abs(gripper - p.gripper));

  return maxDiff;
}

int
Position::setFromString(char* string) {
  bool isSuccess = true;
  int speed;
  setBase(parseInt(string, isSuccess));
  setShoulder(parseInt(string, isSuccess));
  setElbow(parseInt(string, isSuccess));
  setWrist(parseInt(string, isSuccess));
  setWristRotation(parseInt(string, isSuccess));
  setGripper(parseInt(string, isSuccess));
  speed = parseInt(string, isSuccess);
  return isSuccess ? speed : -1;
}

int
Position::parseInt(char *&in, bool &isSuccess) {
  bool success = false;
  int result = 0;
  // Read past separator(s)
  while(((*in > '9') || (*in < '0')) && (*in != 0)) {
    in++;
  }
  // Read integer
  while((*in <= '9') && (*in >= '0')) {
    result *= 10;
    result += (*in - '0');
    in++;
    success = true;
  }
  isSuccess &= success;
  return result;
}
