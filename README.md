![LUMS](https://lums.edu.pk/sites/default/files/inline-images/LUMS%20Logo.png)
# CS100 PROJECT - ASCIIART with OpenCV

## Installation

Precompiled executable is available in `.\ASCIIART\exe\`.

Make sure to add the `opencv_world450.dll` to the environment variables or put it in the same directory as the executable.

## Usage
```code
Usage: [image file path] [size] [low] [high] [black_bg] [text file path]

     image file path    path to the image file you want to convert
                size    number of characters in a row of the converted image (higher == more detail)
                 low    lower pixel value for normalization (0-255)
                high    higher pixel value for normalization (0-255)
            black_bg    whether to have the background of converted image be black or not (0-1)(0 == false; 1 == true)
      text file path    path to the text file where the converted image will be saved

Example: ./myimage.jpg 110 40 240 1 art.txt
```

## Build Instructions

1. Clone the repository
2. Install Visual Studio with "Desktop Development with C++"
3. Open "Developer Powershell for VS 2019"
4. Navigate to the repository
5. Type the following command: `msbuild -p:Configuration=Release .\ASCIIART.sln`

## Where is `opencv_world450.dll`

`.\ASCIIART\opencv\build\x64\vc15\bin\opencv_world450.dll`

## Resources used

- [OpenCV Docs](https://docs.opencv.org/master/modules.html)
- [OpenCV Tutorials](https://docs.opencv.org/master/d9/df8/tutorial_root.html)

#
A project by Muhammad Hamza & Imaan Hameed
