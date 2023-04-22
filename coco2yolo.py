import logging
import os 
import zipfile
logging.getLogger().setLevel(logging.CRITICAL)
from pylabel import importer
from IPython.display import Image, display

os.makedirs("data", exist_ok=True)


#Specify path to the coco.json file
path_to_annotations = "images/annotations.json"
#Specify the path to the images (if they are in a different folder than the annotations)
path_to_images = "images/"

#Import the dataset into the pylable schema 
dataset = importer.ImportCoco(path_to_annotations, path_to_images=path_to_images, name="BCCD_coco")
#dataset.df.head(5)

print(f"Number of images: {dataset.analyze.num_images}")
print(f"Number of classes: {dataset.analyze.num_classes}")
print(f"Classes:{dataset.analyze.classes}")
print(f"Class counts:\n{dataset.analyze.class_counts}")
print(f"Path to annotations:\n{dataset.path_to_annotations}")


#Path to your annotations folder
dataset.path_to_annotations = "images/"
dataset.export.ExportToYoloV5()[0]