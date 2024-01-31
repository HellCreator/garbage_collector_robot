import serial
from time import sleep

command_list_forward = [
    b'x',
    b'm',
    b's',
    b'b'
]
command_list_backward = [      
    b'x',
    b'm',
    b'w',
    b'b'   
 ]
command_list_left = [
    b'x',
    b'm',
    b'd',
    b'b'
 ]
command_list_right = [
    b'x',
    b'm',
    b'a',
    b'b'
 ]
ser = serial.Serial ("/dev/ttyTHS1", 112500)
input_command = input("Insert command")
if input_command == 'f':
  for command in command_list_forward:
      ser.write(command)
      sleep(0.03)
      received_data = ser.readline()
      print(received_data)
      sleep(1)
  ser.close()
elif input_command == 'b':
    for command in command_list_backward:
        ser.write(command)
        sleep(0.03)
        received_data = ser.readline()
        print(received_data)
        sleep(1)
    ser.close()
elif input_command == 'l':
     for command in command_list_left:
         ser.write(command)
         sleep(0.03)
         received_data = ser.readline()
         print(received_data)
         sleep(1)
     ser.close()
else:
    for command in command_list_right:
        ser.write(command)
        sleep(0.03)
        received_data = ser.readline()
        print(received_data)
        sleep(1)
    ser.close()


