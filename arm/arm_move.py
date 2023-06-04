import serial
from time import sleep

'''
b'get_position is\r\n',
b'set_angle 1 0\r\n',
b'set_angle 2 0\r\n',
b'set_angle 3 30\r\n',
b'set_angle 4 -30\r\n',
b'set_angle 5 30\r\n',
b'get_is_moving is\r\n',
b'get_position is\r\n',
b'set_position 10 0 -15 15 -15 15 10\r\n',
b'get_position is\r\n',
b'start_position is\r\n',
'''

command_list = [
    b'power is\r\n',
    b'power on\r\n',
    b'power is\r\n',
    b'get_position is\r\n',
    b'start_position is\r\n',
    b'get_position is\r\n',
    b'set_angle 1 30 \r\n',
    #b'set_angle 2 30 \r\n',
    #b'set_angle 3 10 \r\n',
    #b'set_angle 4 10 \r\n',
    b'set_angle 5 90 \r\n',
    b'set_angle 6 90 \r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'get_position is\r\n',
    b'set_position 10 0 0 0 0 0 10 \r\n',
    b'get_position is\r\n',
    b'start_position is\r\n',
    b'get_joint_maxmin is\r\n',
    b'power off\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
]

ser = serial.Serial ("/dev/ttyUSB0", 112500)
for command in command_list:
    ser.write(command)
    sleep(1)
    received_data = ser.readline()
    #received_data = ser.readlines()
    print(received_data)
    sleep(1)

ser.close() 