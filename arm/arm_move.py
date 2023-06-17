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

command_list_demo = [
    # Returns on (power on) off (poweroff)
    b'power is\r\n',
    # Enable power of robot
    b'power on\r\n',
    b'power is\r\n',
    # Check joint angels
    b'get_position is\r\n',
    # reach coordinates
    b'set_coords_sync 500 300 300 0 0 0 10 1 15 \r\n',
    # set joint  angels to 0 0 0 0 0 0 with speed 10 wait till they reach target or timeout 10
    b'set_angles_sync 0 0 0 0 0 0 10 10 \r\n'
    b'get_position is\r\n',
    b'set_angles_sync 60 -20 30 -40 90 90 10 10 \r\n'
    b'get_position is\r\n',
    # set angels and do not wait till robot finish movement
    b'set_position -60 20 -30 40 0 0 10 \r\n',
    # check if robot moving in this moment True/False
    b'get_is_moving \r\n',
    b'get_position is\r\n',
    b'set_angles_sync 0 0 0 0 0 0 10 10 \r\n'
    b'get_position is\r\n',
    # rest position
    b'set_angles_sync 0 -140 150 -150 -75 0 10 20\r\n'
    b'get_position is\r\n',
    b'set_angles_sync 0 0 0 0 0 0 10 10 \r\n'
    b'get_position is\r\n',
    # move gripper values 0-99 position 50 speed
    b'gripper_status \r\n',
    b'gripper_set value 0 50 \r\n',
    b'gripper_status \r\n',
    b'gripper_set value 20 50 \r\n',
    b'gripper_status \r\n',
    b'gripper_set value 40 50 \r\n',
    b'gripper_status \r\n',
    b'gripper_set value 60 50 \r\n',
    b'gripper_status \r\n',
    # move gripper state 1 closed 0 open 50 speed
    b'gripper_set state 1 50 \r\n',
    b'gripper_status \r\n',
    b'gripper_set state 0 50 \r\n',
    b'set_angles_sync 0 -140 150 -150 -75 0 10 20\r\n'
    b'power off\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
    b'power is\r\n',
]

command_list = [
    b'power on\r\n',
    b'power is\r\n',
    b'set_angles_sync 0 0 0 0 0 0 20 10 \r\n',
    b'gripper_set state 0 50 \r\n',
    b'set_angles_sync 0 -90 0 0 0 45 20 10 \r\n',
    b'gripper_set state 1 50 \r\n',
    b'set_angles_sync 0 0 -90 0 0 45 20 10 \r\n',
    b'set_angles_sync -180 0 -90 0 0 45 20 10 \r\n',
    b'gripper_set state 0 50 \r\n',
    b'set_angles_sync 0 0 0 0 0 0 20 10 \r\n',
    b'set_angles_sync 0 -140 150 -150 -75 0 20 20\r\n'
    b'power off\r\n',
]

ser = serial.Serial("/dev/ttyUSB0", 112500)
for command in command_list:
    ser.write(command)
    sleep(1)
    received_data = ser.readline()
    #received_data = ser.readlines()
    print(received_data)
    sleep(1)

ser.close() 