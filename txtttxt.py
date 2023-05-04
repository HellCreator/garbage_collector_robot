import glob
import os
import json
classes = []
input_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/labels/"
output_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/label/"
image_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/train/images/"
if not os.path.exists(output_dir):
    os.mkdir(output_dir)
files = glob.glob(os.path.join(input_dir, '*.txt'))
# loop through each 
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    # check if the label contains the corresponding image file
    if not os.path.exists(os.path.join(image_dir, f"{filename}.jpg")):
        print(f"{filename} image does not exist!")
        continue
    result = []
    tree = open(str(fil)).readlines()
    for lines in tree:
        lines=list(lines)
        lines[0]=0
        lines = ''.join(map(str, lines))
        result.append(lines)
    label = 'Garbage'
    if label not in classes:
        classes.append(label)
    classes = ''.join(map(str, classes))
    if result:
        with open(os.path.join(output_dir, f"{filename}.txt"), "w", encoding="utf-8") as f:
            f.write("".join(result))
with open(os.path.join(output_dir,'classes.txt'), 'w', encoding='utf8') as f:
    f.write(json.dumps(classes))