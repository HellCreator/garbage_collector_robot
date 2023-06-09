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
timeout = 0

def screen_init():
    setScreenColor(0x222222)
    lcd.clear()
    lcd.fill(0x000000)
    lcd.font(lcd.FONT_Ubuntu)
    lcd.print('Listening UART Commands', 0, 0, 0xffffff)

def parse_command(uart_in, robot, uart_write_func):
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
        elif command[0] == 'gripper_status':
            return 'controller connected ' + str(robot.is_controller_connected()) + '\n gripper value ' + str(robot.get_gripper_value()) + '\n is moving ' + str(robot.is_gripper_moving())
        elif command[0] == 'gripper_set':
            if command[1] == 'state':
                robot.set_gripper_state(int(command[2]), int(command[3]))
                return 'position ' + str(robot.get_gripper_value()) + '\n is moving ' + str(robot.is_gripper_moving())
            elif command[1] == 'value':
                robot.set_gripper_value(int(command[2]), int(command[3]))
                return 'value ' + str(robot.get_gripper_value()) + '\n is moving ' + str(robot.is_gripper_moving())
        elif command[0] == 'start_position':
            uart_write_func('move back to start position....\r\n')
            robot.send_angles([0,0,0,0,0,0], 10)
            timeout = 0
            while robot.is_moving():
                timeout += 1
                if timeout > 100:
                    uart_write_func('movement toooo long\r\n')
                    break
                wait_ms(100)
            return 'retruned to start position [0,0,0,0,0,0], 10 done.'
        elif command[0] == 'get_position':
            return 'angles ' + str(robot.get_angles())
        elif command[0] == 'set_angles_sync':
            uart_write_func('move ' + str(command) + '...\r\n')
            #robot.jog_angle(int(command[1]), int(command[2]), 10)
            robot.sync_send_angles(
                [float(command[1]),float(command[2]),float(command[3]),float(command[4]),float(command[5]),float(command[6])],
                int(command[7]),
                int(command[8]),
                )
            return 'moved ' + str(command[1]) + ' joint to start angle ' + str(command[2]) + ' done.'
        elif command[0] == 'get_joint_maxmin':
            return '1: [' + str(robot.get_joint_min_angle(1)) + ':' + str(robot.get_joint_max_angle(1)) + '] ' + \
                '2: [' + str(robot.get_joint_min_angle(2)) + ':' + str(robot.get_joint_max_angle(2)) + '] ' + \
                '3: [' + str(robot.get_joint_min_angle(3)) + ':' + str(robot.get_joint_max_angle(3)) + '] ' + \
                '4: [' + str(robot.get_joint_min_angle(4)) + ':' + str(robot.get_joint_max_angle(4)) + '] ' + \
                '5: [' + str(robot.get_joint_min_angle(5)) + ':' + str(robot.get_joint_max_angle(5)) + '] ' + \
                '6: [' + str(robot.get_joint_min_angle(6)) + ':' + str(robot.get_joint_max_angle(6)) + ']\r\n'
        elif command[0] == 'get_is_moving':
            return 'True\r\n' if robot.is_moving() else 'False\r\n'
        elif command[0] == 'set_position':
            uart_write_func('move to position....')
            robot.send_angles([float(command[1]),float(command[2]),float(command[3]),float(command[4]),float(command[5]),float(command[6])],int(command[7]))
            timeout = 0
            while robot.is_moving():
                timeout += 1
                if timeout > 30:
                    uart_write_func('movement toooo long\r\n')
                    break
                wait_ms(100)
            return 'position ' + str(robot.get_angles()) + ' done.'
        elif command[0] == 'set_coords_sync':
            '''
            coords: a list of coords value(List[float]).
                        for mycobot :[x(mm), y, z, rx(angle), ry, rz]\n
                        for mypalletizer: [x, y, z, θ]
                        for mypalletizer 340: [x, y, z]
            speed : (int) 0 ~ 100
            mode : (int) 0 - angluar, 1 - linear (mypalletizer 340 does not require this parameter)
            timeout=7
            '''
            uart_write_func('move to coords....')
            robot.sync_send_coords(
                [float(command[1]),float(command[2]),float(command[3]),float(command[4]),float(command[5]),float(command[6])],
                int(command[7]),
                int(command[8]),
                int(command[9]),
                )
            return 'coords ' + str(robot.get_angles()) + ' done.'
    except Exception as e:
        return 'Error during command: ' + str(e)

    return 'Error unsupported command '


def main_run():
    screen_init()
    cobot = mycobot.MyCobot()
    cobot.set_gripper_ini()
    uart2 = machine.UART(2, tx=18, rx=19)
    uart2.init(115200, bits=8, parity=None, stop=1)
    uart2.write("\r\nTEST ROBOARM\r\n")
    while cycle:
        if uart2.any():
            wait_ms(100)
            message = uart2.readline().decode()
            response = parse_command(message, cobot, uart2.write)
            uart2.write(str(response) + "\r\n") 

main_run()
