import cv2
import glob
import os
folder_path = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/images/"
output_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/imag/"
if not os.path.exists(output_dir):
    os.mkdir(output_dir)
files = glob.glob(os.path.join(folder_path, '*.jpg'))
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    im = cv2.imread(os.path.join(folder_path,basename))
    if im.shape[0] > 2000 or im.shape[1]>2000:
        width=2000
        height=(2000/im.shape[0])*im.shape[1]
        dim=(width,height,im.shape[2])
        resized=cv2.resize(os.path.join(folder_path,basename),dim,interpolation = cv2.INTER_LANCZOS4)
        cv2.imwrite(os.path.join(folder_path,basename))
        os.remove(os.path.join(folder_path,basename))