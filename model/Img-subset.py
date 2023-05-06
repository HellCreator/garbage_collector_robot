import glob
import os
import shutil
classes = []
input_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/labels/"
Train_dir="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/Train/images/"
Train_dirl="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/Train/labels/"
Val_dir="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/Val/images/"
Val_dirl="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/Val/labels/"
test_dir="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/test/images/"
test_dirl="C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/Val/labels/"
image_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/images/"
if not os.path.exists(Train_dir):
    os.mkdir(Train_dir)
if not os.path.exists(Train_dirl):
    os.mkdir(Train_dirl)
if not os.path.exists(test_dir):
    os.mkdir(test_dir)
if not os.path.exists(test_dirl):
    os.mkdir(test_dirl)
if not os.path.exists(Val_dir):
    os.mkdir(Val_dir)
if not os.path.exists(Val_dirl):
    os.mkdir(Val_dirl)
files = glob.glob(os.path.join(image_dir, '*.jpg'))
filess = glob.glob(os.path.join(Train_dir, '*.jpg'))
filesss = glob.glob(os.path.join(Val_dir, '*.jpg'))
filessss = glob.glob(os.path.join(test_dir, '*.jpg'))
cTr=len(filess)
cVal=len(filesss)
cTest=len(filessss)
if cTr!= range(0,25000,5):
    for fil in range(0,25000-(cTr*5),5):
        basename = os.path.basename(files[fil])
        filename = os.path.splitext(basename)[0]
        if  os.path.exists(os.path.join(input_dir, f"{filename}.txt")):
            shutil.copyfile(os.path.join(image_dir,basename), os.path.join(Train_dir,basename))
            shutil.copyfile(os.path.join(input_dir,f"{filename}.txt"), os.path.join(Train_dirl,f"{filename}.txt"))
            os.remove(os.path.join(image_dir,basename))
            os.remove(os.path.join(input_dir,f"{filename}.txt"))
if cVal!= range(0,20000,5):
    for fill in range(-1,20000-(cVal*5),5):
        basename = os.path.basename(files[fill])
        filename = os.path.splitext(basename)[0]
        if  os.path.exists(os.path.join(input_dir, f"{filename}.txt")):
            shutil.copyfile(os.path.join(image_dir,basename), os.path.join(Val_dir,basename))
            shutil.copyfile(os.path.join(input_dir,f"{filename}.txt"), os.path.join(Val_dirl,f"{filename}.txt"))
            os.remove(os.path.join(image_dir,basename))
            os.remove(os.path.join(input_dir,f"{filename}.txt"))
if cTest!= range(0,5000,5):
    for fill in range(0,5000-(cTest*5),5):
        basename = os.path.basename(files[fill])
        filename = os.path.splitext(basename)[0]
        if  os.path.exists(os.path.join(input_dir, f"{filename}.txt")):
            shutil.copyfile(os.path.join(image_dir,basename), os.path.join(test_dir,basename))
            shutil.copyfile(os.path.join(input_dir,f"{filename}.txt"), os.path.join(test_dirl,f"{filename}.txt"))
            os.remove(os.path.join(image_dir,basename))
            os.remove(os.path.join(input_dir,f"{filename}.txt"))