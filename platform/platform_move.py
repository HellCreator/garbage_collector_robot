import serial
from time import sleep

command_list = [
    b'x',
    b'w',
    b'b',
]

ser = serial.Serial ("/dev/ttyS0", 112500)
for command in command_list:
    ser.write(command)
    sleep(0.03)
    received_data = ser.readline()
    print(received_data)
    sleep(1)

ser.close() 