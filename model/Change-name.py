import glob
import os
classes = []
input_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/labels/"
image_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/images/"
files = glob.glob(os.path.join(image_dir, '*.jpg'))
files_num=len(files)
i=0;
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    if os.path.exists(os.path.join(input_dir, f"{filename}.txt")):
        if not os.path.exists(os.path.join(input_dir, f"{i}.txt")) & os.path.exists(os.path.join(image_dir, f"{i}.jpg")):
            os.rename(os.path.join(image_dir,basename), os.path.join(image_dir,f"{i}.jpg"))
            os.rename(os.path.join(input_dir,f"{filename}.txt"), os.path.join(input_dir,f"{i}.txt"))
        i+=1