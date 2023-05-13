import cv2 as cv
import glob
import os
folder_path = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/New folder/train/images/"
files = glob.glob(os.path.join(folder_path, '*.jpg'))
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    im = cv.imread(os.path.join(folder_path,basename))
    f=max(im.shape[:1])
    if f>1024:
        index=im.shape.index(f)
        if index==0:
            height=1024
            width=int((1024/im.shape[0])*im.shape[1])
            dim=(width,height)
        else:
            width=1024
            height=int((1024/im.shape[1])*im.shape[0])
            dim=(width,height)
        resized = cv.resize(im, dim, interpolation = cv.INTER_LANCZOS4)
        cv.imwrite(os.path.join(folder_path,f"{filename}.jpg"),resized)