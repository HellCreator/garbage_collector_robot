
import xml.etree.ElementTree as ET
import glob
import os
import json
def xml_to_yolo_bbox(bbox, w, h):
    # xmin, ymin, xmax, ymax
    x_center = ((bbox[1] + bbox[0]) / 2) / w
    y_center = ((bbox[3] + bbox[2]) / 2) / h
    width = (bbox[1] - bbox[0]) / w
    height = (bbox[3] - bbox[2]) / h
    return [x_center, y_center, width, height]
classes = []
input_dir = "C:/Users/mohfi/OneDrive/Desktop/labels/"
output_dir = "C:/Users/mohfi/OneDrive/Desktop/Garbage-Image/DATASET/TRAIN/Val/labels/"
image_dir = "C:/Users/mohfi/Downloads/Compressed/valid/"
os.mkdir(output_dir)
files = glob.glob(os.path.join(input_dir, '*.xml'))
# loop through each 
for fil in files:
    basename = os.path.basename(fil)
    filename = os.path.splitext(basename)[0]
    # check if the label contains the corresponding image file
    if not os.path.exists(os.path.join(image_dir, f"{filename}.jpg")):
        print(f"{filename} image does not exist!")
        continue
    result = []
    # parse the content of the xml file
    tree = ET.parse(fil)
    root = tree.getroot()
    width = int(root.find("size").find("width").text)
    height = int(root.find("size").find("height").text)
    for obj in root.findall('object'):
        label = "Garbage"
        # check for new classes and append to list
        if label not in classes:
            classes.append(label)
        index = classes.index(label)
        pil_bbox = [int(x.text) for x in obj.find("bndbox")]
        yolo_bbox = xml_to_yolo_bbox(pil_bbox, width, height)
        # convert data to string
        bbox_string = " ".join([str(x) for x in yolo_bbox])
        result.append(f"{index} {bbox_string}")
    if result:
        # generate a YOLO format text file for each xml file
        with open(os.path.join(output_dir, f"{filename}.txt"), "w", encoding="utf-8") as f:
            f.write("\n".join(result))
# generate the classes file as reference
with open(os.path.join(output_dir,'classes.txt'), 'w', encoding='utf8') as f:
    f.write(json.dumps(classes))