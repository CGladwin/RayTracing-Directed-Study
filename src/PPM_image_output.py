# Unit 2: Output an Image
import os
import cv2
import matplotlib.pyplot as plt
import math
import numpy as np
import timeit
"""TODO: 
- set up venv
- set up git repo
- start unit 3
- figure out branching so that each unit is finished on its own branch (end of unit 3 calls for big refactor, do this NOW)
- do linting / style checking
- check out dithering 
"""

def view_ppm_img(img_path: str, img_title:str):
    img = cv2.imread(img_path)
    img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
    plt.imshow(img)
    plt.suptitle(img_title)
    plt.show()

if __name__ == "__main__":
    ppm_path = "src/images/rgb_image.ppm"
    view_ppm_img(ppm_path,"first image")
    # print(timeit.timeit('hello_world_ppm_optimized("images")','from __main__ import hello_world_ppm_optimized',number=100))
    # print(timeit.timeit('hello_world_ppm("images")','from __main__ import hello_world_ppm',number=100))
