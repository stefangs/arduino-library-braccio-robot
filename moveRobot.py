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


def move_joint(joint_number, angle):
    positions[current_position].add(joint_number, angle)
    robot.move_to_position(positions[current_position], move_speed)
    joint_labels[joint_number].config(text=str(positions[current_position].get(joint_number)))
    joint_labels[joint_number].update_idletasks()


def change_position(position):
    global current_position, positions, last_position
    if positions[position] is None:
        positions[position] = copy.deepcopy(positions[current_position])
    last_position = current_position
    current_position = position
    robot.move_to_position(positions[current_position], move_speed)
    position_label.config(text=str(position))
    position_label.update_idletasks()
    for j in range(0, len(joints)):
        joint_labels[j].config(text=str(positions[current_position].get(j)))
        joint_labels[j].update_idletasks()


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


def setup_gui():
    global position_label
    root = Tk()
    root.title('Braccio Robot')
    root.bind('<Key>', key_pressed)
    Label(text='Move robot joints with keyboard keys\nChange positions with numerical keys') \
        .grid(row=0, column=0, columnspan=2, padx=(30, 30))
    Label(text='Current position:').grid(row=1, column=0)
    position_label = Label(text=str(current_position))
    position_label.grid(row=1, column=1)

    i = 0
    for joint in joints:
        Label(text=joint).grid(row=2 + i, column=0)
        label = Label(text=str(positions[current_position].get(i)))
        label.grid(row=2 + i, column=1)
        joint_labels.append(label)
        i += 1
    root.mainloop()


joints = ['Base [q,a]', 'Shoulder [w,s]', 'Elbow [e,d]', 'Wrist [r,f]', 'Wrist rotation [t,g]', 'Gripper [y,h]']
home = Position(90, 90, 90, 90, 90, 72)
positions = [copy.deepcopy(home), None, None, None, None, None, None, None, None, None]
current_position = 0
last_position = 0
move_speed = 100
joint_labels = []
position_label = None

port = input("Enter serial port: ")
robot = Braccio(port)
robot.move_to_position(home, 100)
setup_gui()
robot.move_to_position(home, 100)
robot.power_off()
