
# Object Detection based on Color [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE) [![Python 3.6+](https://img.shields.io/badge/python-3.6+-blue.svg)](https://www.python.org/downloads/release/python-360/) ![C++](https://img.shields.io/badge/C++-Solutions-red.svg?style=flat&logo=c++) 


In this repository, an algorithm for Object Detection with OpenCV is introduced, which is based on use of color spaces.
This aims, primarily, to get coordinates, width and height of object using traditional OpenCV algorithms, so this repo doesn't contain what that objects are. 
The presented algorithmus supports:
 - Edge Detection
 - Corner Detection 
 - then Colorful Object Detection.


## Object Detection based on Color 
- Read a color image
- Convert the image to grayscale 

![](img_test2.png) ![](img_test2_r.png)

- Convert the image to binary (i.e. black and white only) using Otsu’s method or a fixed threshold that you choose.

![](bw_img_left.png) ![](bw_img_r.png)

- If the objects in the image are black, and the background is white, we need to invert the image so that white pixels become black and black pixels become white. Otherwise, we leave the image as-is.
- Detect the contours 

![](img_test.png) ![](img_test_right.png)

## Dependencies

 * [numpy]
 * [cython]
 * [OpenCV]

## Setup

This implementation supports python 3.6+.

To use it, you should clone the repository and place it on your local directory.
Now, go into the folder

    "/ObjectDetectionBasedOnColor

The implementation can be compiled in C++ with the file main.cpp
    
    g++ main.cpp objectDetector.cpp -o image `pkg-config --cflags --libs opencv` 

Or, it could be compiled with cython. In that case, perform the following commands on the terminal:

    python3.? setup.py build_ext --inplace