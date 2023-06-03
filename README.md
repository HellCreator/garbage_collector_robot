# Garbage Collector Robot

Robot to colect garbage and deliver it to bin.
Moving platform operated by ATmega128.
With robotic arm mounted on top of it controlled by Raspberry Pi4 with ROS

# Intel Realsense SDK installation Instructions for Raspberry pi 3:
This instructions are specifically to succesfully build the SDK on Raspberry pi model b+ only, not tested in other platforms. Ubuntu 
> **Note**: In the case of RPi 3 model b+ RAM capacity is not sufficient to compile the SDK, so it is recommended to create a swap file of 2GB before trying to compile the SDK , you can get the information in this link [swap file](https://linuxize.com/post/create-a-linux-swap-file/).

## Prepare Raspberry pi 3 

1. Install Raspberry pi Imager: 
   *  [Raspberry Pi Imager](https://www.raspberrypi.com/software/).

2. Install Ubuntu Server 22.04 lts with RPimager:
> **Note**: In the case of you do not have monitor , setup SSH connection and wifi before copying to the sd card [SSH setup and Wifi](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#3-using-advanced-options) .
   * [Ubuntu Server](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#2-prepare-the-sd-card).

## Prepare Ubuntu Installation
1.  Update Ubuntu installation and get the latest kernel:
  *  `sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade`  <br />
2. reboot Raspberry pi 

## Install librealsense:

1.  Open a terminal and do the following:

The following file contains all the commands required to install the SDK , create Swap File and install necessary packages also it will setup the environment for python and remote newtork camera view.!
  *  `wget https://github.com/HellCreator/garbage_collector_robot/edit/main/SDK_Installation/realsense_installation.sh `  <br />
  *  run "chmod +x ./realsense_installation.sh" to make it executable. 
  *  run the script with this command "./realsense_installation.sh"
  *  Wait until the building process finished , you will get a message "Librealsense script completed"
 > **Note**: The previous will install also the python wrappers in case you want to use python in your project , in case you do not want to use python remove the flags "-DBUILD_PYTHON_BINDINGS=true" , "-DPYTHON_EXECUTABLE=/usr/bin/python3" , "-DCMAKE_BUILD_TYPE=Release BUILD_PYTHON_DOCS"  from the script file.
 > > **Note**: The previous will install also the Ethernet camera view Server, in case you do not want to use it remove the flags "DBUILD_NETWORK_DEVICE=ON -DFORCE_RSUSB_BACKEND=ON"from the script file.

2. Once the process is completed connect the Realsense Camera and run "rs-enumerate-devices" to check if the Raspberry pi detects the camera.
3. To test the camera follow the following examples:
  * [C++](https://dev.intelrealsense.com/docs/code-samples).
  * [Python](https://dev.intelrealsense.com/docs/python2).
## Using network camera option:


