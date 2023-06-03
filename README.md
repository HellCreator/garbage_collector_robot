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
3. Wait until it boots. After get the ip address of the Raspberry pi.
4. Connect to the Raspberry pi using the username and password setup in the previous steps.

## Install librealsense:

1.  Open a terminal and do the following:

The following file contains all the commands required to install the SDK , create Swap File and install necessary packages also it will setup the environment for python and remote newtork camera view.!
  *  `wget https://github.com/HellCreator/garbage_collector_robot/edit/main/SDK_Installation/realsense_installation.sh `  <br />
  *  run "chmod +x ./realsense_installation.sh" to make it executable. 
  *  run the script with this command "./realsense_installation.sh"
  *  Wait until the building process finished , you will get a message "Librealsense script completed"
 > **Note**: The previous will install also the python wrappers in case you want to use python in your project , in case you do not want to use python remove the flags "-DBUILD_PYTHON_BINDINGS=true" , "-DPYTHON_EXECUTABLE=/usr/bin/python3" , "-DCMAKE_BUILD_TYPE=Release BUILD_PYTHON_DOCS"  from the script file.
 > **Note**: The previous will install also the Ethernet camera view Server, in case you do not want to use it remove the flags "DBUILD_NETWORK_DEVICE=ON -DFORCE_RSUSB_BACKEND=ON"from the script file.

2. Once the process is completed connect the Realsense Camera and run "rs-enumerate-devices" to check if the Raspberry pi detects the camera.
3. To test the camera follow the following examples:
  * [C++](https://dev.intelrealsense.com/docs/code-samples).
  * [Python](https://dev.intelrealsense.com/docs/python2).

# Using the Realsense Camera over Network

## Using network camera option (Python):

1. In the following folder you will find the files for running the server using python ~/librealsense/wrappers/python/examples/ethernet_client_server 
2. The EtherSenseServer.py has the source code to run the Camera Depth Channel.
3. Once the server is running you need to run the EtherSenseClient.py on the client side.
 > **Note**: If you get the error "TypeError: sequence item 0: expected str instance, bytes found."  just change the line  "self.frame_data = ''.join([length, ts, data])"  to  "self.frame_data = b''.join([length, ts, data])".
5. "Remember the raspberry pi and the client have to be in the same network".

## Using the network camera option (C++) using Windows:

## Run Server in Rasbpberry pi
1.Just run the command "rs-server" you should get a message "Rs-server is running"

## Compile the Realsense Viewer  
1. Download the file [Windows source code.zip](https://github.com/IntelRealSense/librealsense/releases) after that extract the files.
2. Download and install [CMAKE](https://cmake.org/download/)
3. Run CMAKE and open the folder where you extracted the source files using the "Browse Source.
4. Select where you want to build the binaries.
5. Press configure to get the flags.
6. Select the flags "DBUILD_NETWORK_DEVICE=ON ,-DFORCE_RSUSB_BACKEND=ON". and press Generate.

## Compile using Visual Studio
1. Open Visual Studio and load the folder that conatains the files generated in the previous step.
2. Open the folder tools/realsense-viewer.
3. After selecting build the project ,once buid go to the folder Debug , that is where the real-viewer.exe is located.

## Running the Realsense Viewer

1. Run the realsense Viewer and go to Settings.
2. Go to Performace and uncheck the option "Use GLSL for processing" and check "Use GLSL for rendering" 
3. Go to Add Source and Add Network Device. 
8. Connect using the ip address of the Raspberry pi.



