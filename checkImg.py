import glob
import os
import shutil
classes = []
input_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/DATASET/TRAIN\Val/labels/"
output_dir="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/DATASET/TRAIN\Val/image/"
image_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/DATASET/TRAIN\Val/images/"
if not os.path.exists(output_dir):
    os.mkdir(output_dir)
files = glob.glob(os.path.join(image_dir, '*.jpg'))
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    # check if the image has been labelled
    # if the there is a label for the specified image, python will copy the image to the output_dir
    # and delete the image from the input_dir.
    # So, the non_labelled image will remain in the input_dir
    if  os.path.exists(os.path.join(input_dir, f"{filename}.txt")):
        shutil.copyfile(os.path.join(image_dir,basename), f"{os.path.join(output_dir,basename)}")
        os.remove(os.path.join(image_dir,basename))
    