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
* Includes a simple Python program for interactive robot movements

When the robot is moved from one position to another, it starts at a 
specified start speed and accelerates to the specified travel speed. 
Before reaching the end of the movement it decelerates back to the start speed.

`BraccioRobot.moveToPosition(pos, 100);`

The travel speed is specified in degrees per second, for example 100 
degrees per second as in the example above. The speed can be between 
20 and 200 degrees per second.

The start speed is set by the command:

`BraccioRobot.setStartSpeed(speed);`

If the specified start speed is higher than the move speed, then the start 
speed is ignored. This means that the acceleration can be turned off by 
setting start speed to MAXIMUM_SPEED:

`BraccioRobot.setStartSpeed(MAXIMUM_SPEED)`

You can also turn the power to the robot servos on or off by the commands:

`BraccioRobot.powerOn();`

`BraccioRobot.powerOff();`