import socket
import serial
from time import sleep

command_list_forward_slow = [
    b'x',
    b'x',
    b'v -120    ',
    b'b'
]
command_list_forward_med = [
    b'x',
    b'x',
    b'v -135    ',
    b'b'
]
command_list_forward_fast = [
    b'x',
    b'x',
    b'v -150    ',
    b'b'
]
command_list_forward_extrem_fast = [
    b'x',
    b'x',
    b'v -255    ',
    b'b'
]
command_list_backward_slow = [
    b'x',
    b'x',
    b'v 140     ',
    b'b'
]
command_list_backward_med = [
    b'x',
    b'x',
    b'v 150     ',
    b'b'
]
command_list_backward_fast = [
    b'x',
    b'x',
    b'v 185     ',
    b'b'
]
command_list_backward_extrem_fast = [
    b'x',
    b'x',
    b'v 250     ',
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

ser = serial.Serial("/dev/ttyS0", 112500)

def start_server():
    host = '192.168.137.225'
    port = 12345

    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to a specific address and port
    server_socket.bind((host, port))

    # Listen for incoming connections (max 5 connections in the queue)
    server_socket.listen(5)
    print(f"Server listening on {host}:{port}")

    while True:
        # Wait for a connection from a client
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")

        # Receive data from the client
        data = client_socket.recv(1024).decode('utf-8')
        print(f"Received data: {data}")

        # Process the received data (you can customize this part)
        response = process_command(data)

        # Send a response back to the client
        client_socket.send(response.encode('utf-8'))

        # Close the connection with the client
        client_socket.close()
        print("Connection closed.")

def process_command(command):
    # Customize this function based on the commands you want to handle
    if command == 'ff':
        for comm in command_list_forward_extrem_fast:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'f':
        for comm in command_list_forward_fast:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'fm':
        for comm in command_list_forward_med:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'fs':
        for comm in command_list_forward_slow:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'b':
        for comm in command_list_backward_fast:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'bf':
        for comm in command_list_backward_extrem_fast:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'bm':
        for comm in command_list_backward_med:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'bs':
        for comm in command_list_backward_slow:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'l':
        for comm in command_list_left:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    elif command == 'r':
        for comm in command_list_right:
            ser.write(comm)
            sleep(0.03)
            received_data = ser.readline()
            sleep(1)
        return 'finished'
    else:
        return 'invalid command'

if __name__ == "__main__":
    try:
        start_server()
    finally:
        # Close the serial connection outside the loop
        ser.close()
        print("Serial connection closed.")

