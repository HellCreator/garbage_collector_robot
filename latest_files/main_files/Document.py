import subprocess
#subprocess.run(["python", "recognition_with_realsense1.py", "--source 1"])
with open('C:/Users/mohfi/AppData/Local/Programs/Python/yolov5_modified/coordinates.txt', 'r') as f:
    lines = f.readlines()
lines = [int(x) for x in lines[0].split()]
print(lines)
'''
lines = [int(x) for x in lines]
'''