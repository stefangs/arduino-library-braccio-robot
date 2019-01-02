# BraccioRobot
This is a library to control the Tinkerkit Braccio robot arm. The library supports the following features:
* Configurable acceleration and retardation for the movements
* Speeds are adjusted per joint, so all joints reach a new position simultaneously regradless of travel distance
* Can power the arm on and off
* Separate class for handling arm positions
* Configurable start position
* Supports soft start
* Includes example which allows control of the robot via the serial port
* Configurable start and max speed for movements
* Automatically limits speed for the large joints
* Speed specified in degrees per second
* Includes example which gives a serial port API for robot movements
* Includes a simple Python program for interactive robot movements

When the robot is moved from one position to another, it starts at a 
specified start speed and accelerates to the specified travel speed. 
Before reaching the end of the movement it decelerates back to the start speed.

The start speed is set by the command:

`BraccioRobot.setStartSpeed(speed)`

If the specified start speed is higher than the move speed, then the start 
speed is ignored. This means that the acceleration can be turned off by 
setting start speed to MAXIMUM_SPEED:

`BraccioRobot.setStartSpeed(MAXIMUM_SPEED)`
