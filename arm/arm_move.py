import serial
from time import sleep

command_list = [
    b'power is\r\n',
    b'power on\r\n',
    b'power is\r\n',
    b'power off\r\n',
    b'power is\r\n',
]

ser = serial.Serial ("/dev/ttyUSB0", 112500)
for command in command_list:
    ser.write(command)
    sleep(1)
    received_data = ser.readline()
    print(received_data)
    sleep(1)

ser.close() 