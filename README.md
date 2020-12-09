![LUMS](https://lums.edu.pk/sites/default/files/inline-images/LUMS%20Logo.png)
# CS100 PROJECT - ASCIIART with OpenCV

## Installation

1. Clone or download the git repository
2. Add the `opencv_world450.dll` file to environment variables or put the dll in the same directory as the executable
3. [OPTIONAL] If you get any error while running the program then download and install the VS C++ Runtime from this [link](
https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

## Usage
```code
Usage: [image file path] [size] [low] [high] [black_bg] [text file path]

     image file path    path to the image file you want to convert
                size    number of characters in a row of the converted image (higher == more detail)
                 low    lower pixel value for normalizatiBon (0-255)
                high    higher pixel value for normalization (0-255)
            black_bg    whether to have the backgound of convertted image be black or not (0-1)(0 == false; 1 == true)
      text file path    path to the text file where the converted image will be saved

Example: ./myimage.jpg 110 40 240 1 art.txt
```

## Resources used

- (OpenCV Docs & Tutorials)[https://docs.opencv.org/master/d9/df8/tutorial_root.html]