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

def progress_indicator(base_text: str,start_value: int,current_value: int) -> None:
    """overrides previous string with current value + spaces based on length of initial value"""
    # used to indicate progress of loop
    def get_digits(number):
        '''helper function that outperforms len(str(number))'''
        # https://stackoverflow.com/questions/2189800/how-to-find-length-of-digits-in-an-integer
        if number > 0:
            return int(math.log10(number))+1
        elif number == 0:
            return 1
        else:
            raise ValueError("progress indicator should not reach negative numbers")
    spaces_necessary = get_digits(start_value)-get_digits(current_value)
    print("\r%s%d" % (base_text,current_value)+" "*spaces_necessary,end="",flush=True)

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
