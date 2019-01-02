
/*
  positions.ino

 This sketch shows how to move the Braccio to different positions
 with different options. The example picks up an object at a pickup
 position and puts it down at the put down position and then moves it
 back again.

 Created 01012019
 by Stefan Strömberg

 This example is in the public domain.
 */

#include <BraccioRobot.h>
#include <Servo.h>
#define GRIPPER_HALF_CLOSED 64

// Create a custom initial position
// M1=base degrees. Allowed values from 0 to 180 degrees
// M2=shoulder degrees. Allowed values from 15 to 165 degrees
// M3=elbow degrees. Allowed values from 0 to 180 degrees
// M4=wrist degrees. Allowed values from 0 to 180 degrees
// M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
// M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
//                        (M1, M2, M3, M4, M5,  M6)
Position myInitialPosition(90, 90, 90, 90, 45,  GRIPPER_CLOSED);

Position readyPosition(87, 135, 0, 57, 90, 10);
Position pickUpPosition(120, 84, 0, 42, 90, 10);
Position putDownPosition;

void setup() {
  //init() will start the robot and move to the supplied initial position.
  // If no initial position is supplied, it will go to the default initial position
  BraccioRobot.init(myInitialPosition);
  
  // The put down position is just 60 degrees rotated from the pick up position
  putDownPosition = pickUpPosition;
  putDownPosition.setBase(60);
}

// Moves an object between two specified positions
void moveBetweenPositions(Position& fromPosition, Position& toPosition) {
  
  // Move to the ready position with open gripper
  BraccioRobot.moveToPosition(readyPosition.setGripper(GRIPPER_OPEN), 100);

  // Move to the fromPosition with open gripper
  BraccioRobot.moveToPosition(fromPosition.setGripper(GRIPPER_OPEN), 100);  
  
  // Close the gripper slowly at fromPosition
  BraccioRobot.moveToPosition(fromPosition.setGripper(GRIPPER_HALF_CLOSED), 20);  
  
  // Move at moderate speed to the ready position with closed gripper
  BraccioRobot.moveToPosition(readyPosition.setGripper(GRIPPER_HALF_CLOSED), 50);

  // Move at moderate speed to the toPosition with closed gripper
  BraccioRobot.moveToPosition(toPosition.setGripper(GRIPPER_HALF_CLOSED), 50);

  // Open the gripper slowly at the toPosition 
  BraccioRobot.moveToPosition(toPosition.setGripper(GRIPPER_OPEN), 20);

  // Move to the ready position with open gripper
  BraccioRobot.moveToPosition(readyPosition.setGripper(GRIPPER_OPEN), 100);   
}

void loop() {

  // Move from pickUpPosition to putDownPosition
  moveBetweenPositions(pickUpPosition, putDownPosition);

  // Move back from putDownPosition to pickUpPosition
  moveBetweenPositions(putDownPosition, pickUpPosition);

  //Wait 5 seconds
  delay(5000);
}
