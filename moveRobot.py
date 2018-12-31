import serial
import time
import copy
from tkinter import *


class Position:
    def __init__(self, base, shoulder, elbow, wrist, wrist_rotation, gripper):
        self.angels = [0, 0, 0, 0, 0, 0]
        self.angels[0] = base
        self.angels[1] = shoulder
        self.angels[2] = elbow
        self.angels[3] = wrist
        self.angels[4] = wrist_rotation
        self.angels[5] = gripper

    def set(self, joint_number, value):
        self.angels[joint_number] = min(max(value, 0), 180)

    def get(self, joint_number):
        return self.angels[joint_number]

    def add(self, joint_number, value):
        self.set(joint_number, self.angels[joint_number] + value)

    def to_string(self):
        result = ""
        separator = ""
        for angel in self.angels:
            result = result + separator + str(angel)
            separator = ","
        return result


class Braccio:
    def __init__(self, serial_port):
        self.port = serial.Serial(serial_port, 115200, timeout=5)
        time.sleep(3)

    def write(self, string):
        self.port.write(string.encode())
        self.port.readline()

    def move_to_position(self, position, speed):
        self.write('P' + position.to_string() + ',' + str(speed) + '\n')

    def power_off(self):
        self.write('0\n')

    def power_on(self):
        self.write('0\n')


moves = {'q': (0, 1), 'a': (0, -1),
         'w': (1, 1), 's': (1, -1),
         'e': (2, 1), 'd': (2, -1),
         'r': (3, 1), 'f': (3, -1),
         't': (4, 1), 'g': (4, -1),
         'y': (5, 1), 'h': (5, -1)}


position_names = {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}


def move_joint(joint, angle):
    positions[current_position].add(joint, angle)
    print(positions[current_position].to_string())
    robot.move_to_position(positions[current_position], move_speed)


def change_position(position):
    global current_position, positions, last_position
    if positions[position] is None:
        positions[position] = copy.deepcopy(positions[current_position])
    last_position = current_position
    current_position = position
    robot.move_to_position(positions[current_position], move_speed)
    print('Current position:', current_position)


def key_pressed(event):
    global positions
    if event.char in moves:
        move = moves[event.char]
        move_joint(move[0], move[1])
    elif event.char in position_names:
        change_position(position_names[event.char])
    elif event.char == "c":
        positions[current_position] = copy.deepcopy(positions[last_position])
        change_position(current_position)


home = Position(90, 90, 90, 90, 90, 72)
positions = [copy.deepcopy(home), None, None, None, None, None, None, None, None, None]
current_position = 0
last_position = 0
move_speed = 100
robot = Braccio('COM4')
robot.move_to_position(home, 100)

root = Tk()
root.bind('<Key>', key_pressed)

root.mainloop()

robot.move_to_position(home, 100)
robot.power_off()
