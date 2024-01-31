#!/usr/bin/env python
import os, sys
import cv2
import pyrealsense2 as rs
import numpy as np
import time
from pathlib import Path
import torch
import torch.backends.cudnn as cudnn
from math import *

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
def run():

    weights='best-4.pt'  # model.pt path(s)
    imgsz=640  # inference size (pixels)
    conf_thres=0.25  # confidence threshold
    iou_thres=0.45  # NMS IOU threshold
    max_det=1  # maximum detections per image
    classes=None  # filter by class: --class 0, or --class 0 2 3
    agnostic_nms=False  # class-agnostic NMS
    augment=False  # augmented inference
    visualize=False  # visualize features
    line_thickness=3  # bounding box thickness (pixels)
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
        model(torch.zeros(1, 3, imgsz, imgsz).to(device).type_as(next(model.parameters())))  # run once

    config = rs.config()
    config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
    config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

    pipeline = rs.pipeline()
    profile = pipeline.start(config)

    align_to = rs.stream.color
    align = rs.align(align_to)
    intr = profile.get_stream(rs.stream.color).as_video_stream_profile().get_intrinsics()
    while(True):
        t0 = time.time()

        frames = pipeline.wait_for_frames()

        aligned_frames = align.process(frames)
        color_frame = aligned_frames.get_color_frame()
        depth_frame = aligned_frames.get_depth_frame()
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
        img = img[np.newaxis, :, :, :]        

        # Stack
        img = np.stack(img, 0)

        # Convert
        img = img[..., ::-1].transpose((0, 3, 1, 2))  # BGR to RGB, BHWC to BCHW
        img = np.ascontiguousarray(img)

        img = torch.from_numpy(img).to(device)
        img = img.half() if half else img.float()  # uint8 to fp16/32
        img /= 255.0  # 0 - 255 to 0.0 - 1.0
        if img.ndimension() == 3:
            img = img.unsqueeze(0)

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
                    #'''
                    if x != [] or y != []:
                        dist = depth_frame.get_distance(x, y)*100
                        Xtarget = dist*(x  - intr.ppx)/intr.fx #the distance from RGB camera to realsense center
                        lines.append(Xtarget)
                        Ytarget = dist*(y  - intr.ppy)/intr.fy
                        lines.append(Ytarget)
                        Ztarget = dist
                        lines.append(Ztarget)
                        hfov = 2*degrees(atan(640/(2*intr.fx)))
                        if (x >= 400):
                            theta = (((x-400)/(640-400))*(hfov*((640-400)/640)))
                        else:
                            theta = (((x-400)/(400))*(hfov*((400)/640)))
                        lines.append(theta)
                        print( "X = ",Xtarget," Y = ", Ytarget, " Z = ", Ztarget, " theta = ", theta)
                        coordinate_text = " "+ " X " + " = " + str(round(Xtarget)) + ", " + " Y " + " = " + str(round(Ytarget)) + ", " + " Z " + " = " + str(round(Ztarget)) + ", " + " Theta " + " = " + str(round(theta))
                        lines = [(round(Xtarget)),(round(Ytarget)),(round(Ztarget)),(round(theta))]
                        #print (lines)
                        with open('coordinates.txt', 'w') as f:
                            for i in range(len(lines)):
                                if i == 0:
                                    f.write('%d' % lines[i])
                                else:
                                    f.write(", %d" % lines[i])
                        f.close()
                        cv2.putText(img0, text=coordinate_text, org=(int((xyxy[0] + xyxy[2])/2), int((xyxy[1] + xyxy[3])/2)),
                        fontFace = font, fontScale = .5, color=(255,255,255), thickness=2, lineType = cv2.LINE_AA)
                        cv2.putText(depth_colormap, text=coordinate_text, org=(int((xyxy[0] + xyxy[2])/2), int((xyxy[1] + xyxy[3])/2)),
                        fontFace = font, fontScale = .5, color=(255,255,255), thickness=2, lineType = cv2.LINE_AA)
                        img0 = cv2.circle(img0, (x, y), radius=10, color=(0, 0, 255), thickness=-1)
                        depth_colormap = cv2.circle(depth_colormap, (x, y), radius=10, color=(255, 255, 255), thickness=-1)
        cv2.imshow("IMAGE", img0)
        cv2.imshow("DEPTH", depth_colormap)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break  

if __name__ == '__main__':
    run()