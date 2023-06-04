from m5stack import *
from m5ui import *
from uiflow import *
from ThirdParty.myCobot import mycobot

cycle = True
response = None
command = None
cobot = None
error_message= str()
message = None

def screen_init():
    setScreenColor(0x222222)
    lcd.clear()
    lcd.fill(0x000000)
    lcd.font(lcd.FONT_Ubuntu)
    lcd.print('Listening UART Commands', 0, 0, 0xffffff)

def parse_command(uart_in, robot):
    try:
        command = str(uart_in).split(' ')
        if command[0] == 'power':
            if command[1] == 'on\r\n':
                robot.power_on()
                return 'power on'
            elif command[1] == 'off\r\n':
                robot.power_off()
                return 'power off'
            else:
                return 'power is ' + 'on' if robot.is_power_on() else 'off'
    except Exception as e:
        return 'Error during command: ' + str(e)

    return 'Error unsupported command '


def main_run():
    screen_init()
    cobot = mycobot.MyCobot()
    uart2 = machine.UART(2, tx=18, rx=19)
    uart2.init(115200, bits=8, parity=None, stop=1)
    uart2.write("\r\nTEST ROBOARM\r\n")
    while cycle:
        if uart2.any():
            wait_ms(100)
            message = uart2.readline().decode()
            response = parse_command(message, cobot)
            #uart2.write('echo ')
            #uart2.write(message)
            uart2.write(str(response) + "\r\n") 

main_run()