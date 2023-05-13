import cv2
import glob
import os
folder_path = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/New folder/train/images/"
output_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/New folder/train/imag/"
if not os.path.exists(output_dir):
    os.mkdir(output_dir)
files = glob.glob(os.path.join(folder_path, '*.jpg'))
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    im = cv2.imread(os.path.join(folder_path,basename))
    if im.shape[0] > 1024 or im.shape[1]>1024:
        f=max(im.shape[:1])
        index=im.shape.index(f)
        if index==0:
            height=1024
            width=int((1024/im.shape[0])*im.shape[1])
            dim=(width,height)
        else:
            width=1024
            height=int((1024/im.shape[1])*im.shape[0])
            dim=(width,height)
        resized = cv2.resize(im, dim, interpolation = cv2.INTER_LANCZOS4)
        cv2.imwrite(os.path.join(output_dir,f"{filename}.jpg"),resized)