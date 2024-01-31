import serial
from time import sleep

command_list =[b'power on\r\n',
             b'get_position is\r\n',
             b'gripper_set state 1 100 \r\n',
             b'set_angles_sync -90 0 0 0 0 0 20 20\r\n',
             b'set_angles_sync 0 0 0 0 0 0 20 20\r\n',
             b'gripper_set state 0 50 \r\n',
             b'set_angles_sync 0 -90 -20 35 0 50 20 20\r\n',
             b'power is\r\n',
	     b'gripper_set state 1 50 \r\n',
             b'set_angles_sync 0 -10 0 -75 0 45 20 22\r\n',
             b'set_angles_sync 140 0 0 -75 -10 45 20 20\r\n',
             b'gripper_set state 0 100 \r\n',
             b'power is\r\n',
             b'power is\r\n',
             b'gripper_set state 1 100 \r\n',
             b'set_angles_sync -40 0 0 0 0 0 20 20\r\n',
             b'power is\r\n',
             b'power is\r\n',
             b'gripper_status \r\n',
            b'gripper_set state 1 100 \r\n',
    	     b'set_angles_sync -70 -130 155 -125 90 20 20 20\r\n',
    	     b'power is\r\n',
    	     b'power is\r\n',
    	     b'power is\r\n',  
    ]
#Variable ser connects to USB port 
ser = serial.Serial("/dev/ttyUSB0", 112500)

    
for command in command_list:
    ser.write(command)
    sleep(1)
    received_data = ser.readline()
    print(received_data)
    sleep(1)

ser.close()

