from PIL import Image
import os
folder_path = 'D:/Gdansk University of Technology/Garbage Collector Robot/Images/'
output= 'D:/Gdansk University of Technology/Garbage Collector Robot/resized/'
for root, dirs, files in os.walk(folder_path):
    for filename in files:
        full_path = os.path.join(root, filename)
        im = Image.open(full_path)
        imResize = im.resize((1024, 768))
        imResize.save(os.path.join(output, '.jpg'))



