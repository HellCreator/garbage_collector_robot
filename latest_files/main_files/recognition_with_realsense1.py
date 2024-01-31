#!/usr/bin/env python
import os, sys
import socket
import cv2
import pyrealsense2 as rs
import numpy as np
import time
from pathlib import Path
import torch
import torch.backends.cudnn as cudnn
from math import *
import serial
FILE = Path(__file__).absolute()
sys.path.append(FILE.parents[0].as_posix())

from models.experimental import attempt_load
from utils.datasets import LoadStreams, LoadImages
from utils.augmentations import Albumentations, augment_hsv, copy_paste, letterbox, mixup, random_perspective
from utils.general import check_img_size, check_requirements, check_imshow, colorstr, non_max_suppression, \
    apply_classifier, scale_coords, xyxy2xywh, strip_optimizer, set_logging, increment_path, save_one_box
from utils.plots import Annotator, colors
from utils.torch_utils import load_classifier, select_device, time_sync

font = cv2.FONT_HERSHEY_SIMPLEX

@torch.no_grad()
def arm_move(theta):
    print(theta)
    command_list =[b'power on\r\n',
             b'get_position is\r\n',
             b'gripper_set state 1 100 \r\n',
             b'set_angles_sync -90 0 0 0 0 0 20 20\r\n',
             b'set_angles_sync 0 0 0 0 0 0 20 20\r\n',
             b'gripper_set state 0 50 \r\n',
             b'set_angles_sync %d -90 -20 35 0 50 20 20\r\n'%(theta + 15),
             b'power is\r\n',
	     b'gripper_set state 1 50 \r\n',
             b'set_angles_sync %d -10 0 -75 0 45 20 22\r\n'%(theta + 15),
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
        time.sleep(1)
        received_data = ser.readline()
        print(received_data)
        time.sleep(1)

    ser.close()

def platform_move(distance):

    command_f_36 = 'ff'
    command_f_10= 'f'
    command_f_6 = 'fm'
    command_f_1 = 'fs'
    command_b = 'bm'
    server_address = '192.168.137.225'
    server_port = 12345
    print("Distance = ", distance)
    if round(distance,1) >= 1.7:
        if round(distance)>=4:
            for x in range(2):
                # Define the server address and port
                # Create a socket object
                client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                # Connect to the server
                client_socket.connect((server_address, server_port))
                # Send the command to the server
                client_socket.send(command_f_36.encode('utf-8'))
                # Receive and print the response from the server
                response = client_socket.recv(1024).decode('utf-8')
                print(f"Server Response: {response}")
                # Close the connection with the server       
                #client_socket.shutdown(socket.SHUT_WR)
                client_socket.close()
                print("Connection closed.")
        else:
            # Define the server address and port
            # Create a socket object
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            client_socket.connect((server_address, server_port))

            # Send the command to the server
            client_socket.send(command_f_36.encode('utf-8'))

            # Receive and print the response from the server
            response = client_socket.recv(1024).decode('utf-8')
            print(f"Server Response: {response}")

            # Close the connection with the server
               
            #client_socket.shutdown(socket.SHUT_WR)
        client_socket.close()
        print("Connection closed.")
    elif round(distance,1) < 1.7 and round(distance,1) >= 1:
        for x in range(round(distance)):
            # Define the server address and port
            # Create a socket object
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            client_socket.connect((server_address, server_port))

            # Send the command to the server
            client_socket.send(command_f_10.encode('utf-8'))

            # Receive and print the response from the server
            response = client_socket.recv(1024).decode('utf-8')
            print(f"Server Response: {response}")

            # Close the connection with the server
               
            #client_socket.shutdown(socket.SHUT_WR)
        client_socket.close()
        print("Connection closed.")
    elif round(distance,1) < 1 and round(distance,1) >= .5:
        for x in range(round(distance)+1):
            # Define the server address and port
            # Create a socket object
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            client_socket.connect((server_address, server_port))

            # Send the command to the server
            client_socket.send(command_f_1.encode('utf-8'))

            # Receive and print the response from the server
            response = client_socket.recv(1024).decode('utf-8')
            print(f"Server Response: {response}")

            # Close the connection with the server
               
            #client_socket.shutdown(socket.SHUT_WR)
        client_socket.close()
        print("Connection closed.")
    elif round(distance,1) < .5 and round(distance,1) > 0:
        for x in range(round(distance)+1):
            # Define the server address and port
            # Create a socket object
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            client_socket.connect((server_address, server_port))

            # Send the command to the server
            client_socket.send(command_f_6.encode('utf-8'))

            # Receive and print the response from the server
            response = client_socket.recv(1024).decode('utf-8')
            print(f"Server Response: {response}")

            # Close the connection with the server
               
            #client_socket.shutdown(socket.SHUT_WR)
        client_socket.close()
        print("Connection closed.")
    elif distance < 0:

      # Define the server address and port
      # Create a socket object
      client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

      # Connect to the server
      client_socket.connect((server_address, server_port))

      # Send the command to the server
      client_socket.send(command_b.encode('utf-8'))

      # Receive and print the response from the server
      response = client_socket.recv(1024).decode('utf-8')
      print(f"Server Response: {response}")

      # Close the connection with the server
      client_socket.close()

def run():

    weights='best-8.pt'  # model.pt path(s)
    imgsz=[448,256]  # inference size (pixels)
    conf_thres=0.25  # confidence threshold
    iou_thres=0.45  # NMS IOU threshold
    max_det = 1 # maximum detections per image
    classes=None  # filter by class: --class 0, or --class 0 2 3
    agnostic_nms=False  # class-agnostic NMS
    augment=False  # augmented inference
    visualize=False  # visualize features
    line_thickness = 3  # bounding box thickness (pixels)
    hide_labels=False  # hide labels
    hide_conf=False  # hide confidences
    half=False  # use FP16 half-precision inference
    stride = 32
    device = 0  # cuda device, i.e. 0 or 0,1,2,3 or cpu
    view_img=False  # show results
    save_crop=False  # save cropped prediction boxes
    nosave=False  # do not save images/videos
    update=False  # update all models
    name='exp'  # save results to project/name

    # Initialize
    set_logging()
    device = select_device(device)
    half &= device.type != 'cpu'  # half precision only supported on CUDA

    # Load model
    model = attempt_load(weights, map_location = device)  # load FP32 model
    stride = int(model.stride.max())  # model stride
    imgsz = check_img_size(imgsz, s=stride)  # check image size
    names = model.module.names if hasattr(model, 'module') else model.names  # get class names
    if half:
        model.half()  # to FP16

    # Second-stage classifier
    classify = False
    if classify:
        modelc = load_classifier(name='resnet50', n=2)  # initialize
        modelc.load_state_dict(torch.load('resnet50.pt', map_location = device)['model']).to(device).eval()

    # Dataloader
    view_img = check_imshow()
    cudnn.benchmark = True  # set True to speed up constant image size inference

    # Run inference
    if device.type != 'cpu':
        model(torch.zeros(1, 3, imgsz[0], imgsz[1]).to(device).type_as(next(model.parameters())))  # run once

    config = rs.config()
    config.enable_stream(rs.stream.color, 424, 240, rs.format.bgr8, 30)
    config.enable_stream(rs.stream.depth, 424, 240, rs.format.z16, 30)

    pipeline = rs.pipeline()
    profile = pipeline.start(config)

    align_to = rs.stream.color
    align = rs.align(align_to)
    intr = profile.get_stream(rs.stream.color).as_video_stream_profile().get_intrinsics()
    numofframes = 0
    while(True):
        t0 = time.time()

        frames = pipeline.wait_for_frames()

        aligned_frames = align.process(frames)
        depth_frame = aligned_frames.get_depth_frame()
        color_frame = aligned_frames.get_color_frame()
    
        if not depth_frame or not color_frame:
            continue

        img = np.asanyarray(color_frame.get_data())
        depth_image = np.asanyarray(depth_frame.get_data())
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.08), cv2.COLORMAP_JET)
        # check for common shapes
        s = np.stack([letterbox(x, imgsz, stride=stride)[0].shape for x in img], 0)  # shapes
        rect = np.unique(s, axis=0).shape[0] == 1  # rect inference if all shapes equal
        if not rect:
            print('WARNING: Different stream shapes detected. For optimal performance supply similarly-shaped streams.')

        # Letterbox
        img0 = img.copy()
        padding_value = 0
        img = np.pad(img,
                        ((0,16),  # pad bottom
                            (0,24),  # pad right
                            (0, 0)),  # don't pad channels
                        mode='constant',
                        constant_values=(((padding_value, padding_value, padding_value), (padding_value, padding_value, padding_value)), ((padding_value, padding_value, padding_value), (padding_value, padding_value, padding_value)), (0, 0)))
        img = img[np.newaxis, :, :, :]        
        #Stack
        img = np.stack(img, 0)
        #img = img[0:287,19:467]
        # Convert
        img = img[..., ::-1].transpose((0, 3, 1, 2))  # BGR to RGB, BHWC to BCHW
        img = np.ascontiguousarray(img)
        img = torch.from_numpy(img).to(device)
        img = img.half() if half else img.float()  # uint8 to fp16/32
        img /= 255.0  # 0 - 255 to 0.0 - 1.0
        if len(img.shape) == 3:
            img = img[None]
        # Inference
        t1 = time_sync()
        pred = model(img, augment=augment,
                        visualize=increment_path(save_dir / 'features', mkdir=True) if visualize else False)[0]

        # Apply NMS
        pred = non_max_suppression(pred, conf_thres, iou_thres, classes, agnostic_nms, max_det=max_det)
        t2 = time_sync()

        # Apply Classifier
        if classify:
            pred = apply_classifier(pred, modelc, img, img0)

        # Process detections
        lines = []
        Ztarget, Xtarget, Ytarget, theta = 0, 0, 0, 0
        for i, det in enumerate(pred):  # detections per image
            s = f'{i}: '
            s += '%gx%g ' % img.shape[2:]  # print string
            annotator = Annotator(img0, line_width=line_thickness, example=str(names))
            annotator1 = Annotator(depth_colormap, line_width=line_thickness, example=str(names))
            if len(det):
                # Rescale boxes from img_size to im0 size
                det[:, :4] = scale_coords(img.shape[2:], det[:, :4], img0.shape).round()

                # Print results
                for c in det[:, -1].unique():
                    n = (det[:, -1] == c).sum()  # detections per class
                    s += f"{n} {names[int(c)]}{'s' * (n > 1)}, "  # add to string
                for *xyxy, conf, cls in reversed(det):
                    lines = []
                    x,y=[],[]
                    c = int(cls)  # integer class
                    label = None if hide_labels else (names[c] if hide_conf else f'{names[c]} {conf:.2f}')
                    annotator.box_label(xyxy, label, color=colors(c, True))
                    annotator1.box_label(xyxy, label, color=colors(c, True))
                    x = int((xyxy[0] + xyxy[2])/2)
                    y = int((xyxy[1] + xyxy[3])/2)
                    if x != [] or y != []:
                        dist = depth_frame.get_distance(x, y)*100
                        Xtarget = dist*(x  - intr.ppx)/intr.fx #the distance from RGB camera to realsense center
                        lines.append(Xtarget)
                        Ytarget = dist*(y  - intr.ppy)/intr.fy
                        lines.append(Ytarget)
                        Ztarget = dist
                        lines.append(Ztarget)
                        hfov = 2*degrees(atan(424/(2*intr.fx)))
                        theta = (((-x+intr.ppx)/(424))*(hfov*((424-intr.ppx)/intr.ppx)))
                        lines.append(theta)
                        print( "X = ",Xtarget," Y = ", Ytarget, " Z = ", Ztarget, " theta = ", theta)
                        coordinate_text = " "+ " X " + " = " + str(round(Xtarget)) + ", " + " Y " + " = " + str(round(Ytarget)) + ", " + " Z " + " = " + str(round(Ztarget)) + ", " + " Theta " + " = " + str(round(theta))
                        lines = [(round(Xtarget)),(round(Ytarget)),(round(Ztarget)),(round(theta))]
                        #print (lines)
                        '''
                        with open('coordinates.txt', 'w') as f:
                            for i in range(len(lines)):
                                if i == 0:
                                    f.write('%d' %lines[i])
                                else:
                                    f.write(" %d" %lines[i])
                        f.close()
                        '''
                        cv2.putText(img0, text=coordinate_text, org=(int((xyxy[0] + xyxy[2])/2), int((xyxy[1] + xyxy[3])/2)),
                        fontFace = font, fontScale = .5, color=(255,255,255), thickness=2, lineType = cv2.LINE_AA)
                        cv2.putText(depth_colormap, text=coordinate_text, org=(int((xyxy[0] + xyxy[2])/2), int((xyxy[1] + xyxy[3])/2)),
                        fontFace = font, fontScale = .5, color=(255,255,255), thickness=2, lineType = cv2.LINE_AA)
                        img0 = cv2.circle(img0, (x, y), radius=10, color=(0, 0, 255), thickness=-1)
                        depth_colormap = cv2.circle(depth_colormap, (x, y), radius=10, color=(255, 255, 255), thickness=-1)
        cv2.imshow("IMAGE", img0)
        #cv2.imshow("IMAGE", img1)
        cv2.imshow("DEPTH", depth_colormap)
        #print ("N = ",numofframes," N = ",numofframes, " N = ", numofframes, " N = ", numofframes)
        if numofframes == 10:
            numofframes = 0
            if round(Ztarget)==20 and len(lines) == 4 :
                arm_move(lines[-1])
                time.sleep(3)
            elif round(Ztarget) >= 21 and len(lines) == 4:
                #platform_move(lines[-2])
                 distance = ((Ztarget - 20) / 20)
                 platform_move(distance)
                 time.sleep(3)
            elif round(Ztarget) <= 19 and len(lines) == 4:
                 distance = -round(Ztarget / 20)
                 platform_move(distance)
                 time.sleep(3)
           
        else:
            numofframes = numofframes+1
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

if __name__ == '__main__':
    run()
