import serial
from time import sleep

command_list = [
    b'x',
    b's',
    b'b',
    b'm',
    b'v -200    ',
    b'c 200    ',
    b'e -200   ',
    b'v -150    ',
    b'v -150    ',
    b'v -200    ',
    b'c -200   ',
    b'v -200    ',
    b'c -200   ',
    b'b'
]

command_list2 = [
    b'x',
    b's',
    b'x',
    b'x',
    b'x',
    b'x',
    b'x',
    b'b',
]

command_list3 = [
    b'b',
    b'w',
    b'x',
    b'x',
    b'x',
    b'x',
    b'x',
    b'b'
]

ser = serial.Serial ("/dev/ttyS0", 112500)
for command in command_list3:
    ser.write(command)
    sleep(0.03)
    received_data = ser.readline()
    print(received_data)
    sleep(1)

ser.close() 