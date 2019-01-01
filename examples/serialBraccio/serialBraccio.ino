/*
  File: serialBraccio.ino

  Copyright 2018 Stefan Strömberg, stefangs@nethome.nu

  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
  in compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied. See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
  This example reads robot positions from the serial port and puts
  the Braccio in those positions. A position is formatted as:

  P90,90,90,90,90,73,100<NL>

  Where the numbers are the joint angels in the same order as in the Position class and the last
  number is the speed of the movement in degrees per second.
  When the arm has reached the specified position it will answer with the string:

  OK<NL>

  You can also issue the Home command (H), to put the robot in the home position, the Off command (0)
  to turn off the robot or the On command (1) to turn on the robot:

  H<NL>

  Note, that when opening the serial port to Arduino, the Arduino restarts, which causes
  it to become non responsive for serial input for a couple of seconds. Therefore a
  controller program should wait at least 3 seconds after opening the port before sending
  any commands. An example in Python:

  s = serial.Serial('COM4', 115200, timeout=5)
  time.sleep(3)
  s.write(b'P0,90,20,90,90,73,100\n')
  print(s.readline())
  s.write(b'P90,90,20,90,90,73,100\n')
  print(s.readline())

  You should also wait until you get an "OK" (or "E0") from the Arduino before sending the
  next position, like in the above example using readline().
*/

#include <Servo.h>
#include "BraccioRobot.h"
#define INPUT_BUFFER_SIZE 50

static char inputBuffer[INPUT_BUFFER_SIZE];
Position armPosition;

void setup() {
  Serial.begin(115200);
  BraccioRobot.init();
}

void loop() {
    handleInput();
}

void handleInput() {
  if (Serial.available() > 0) {
    byte result = Serial.readBytesUntil('\n', inputBuffer, INPUT_BUFFER_SIZE);
    inputBuffer[result] = 0;
    interpretCommand(inputBuffer, result);
  }
}

void interpretCommand(char* inputBuffer, byte commandLength) {
  if (inputBuffer[0] == 'P') {
    positionArm(&inputBuffer[0]);
  } else if (inputBuffer[0] == 'H') {
    homePositionArm();
  } else if (inputBuffer[0] == '0') {
    BraccioRobot.powerOff();
    Serial.println("OK");
  }  else if (inputBuffer[0] == '1') {
    BraccioRobot.powerOn();
    Serial.println("OK");
  } else {
    Serial.println("E0");
  }
  Serial.flush();
}

void
positionArm(char *in) {
  int speed = armPosition.setFromString(in);
  if (speed > 0) {
    BraccioRobot.moveToPosition(armPosition, speed);
    Serial.println("OK");
  } else {
    Serial.println("E1");
  }
}

void
homePositionArm() {
  BraccioRobot.moveToPosition(armPosition.set(90, 90, 90, 90, 90, 73), 100);
  Serial.println("OK");
}
