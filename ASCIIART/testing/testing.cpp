#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <fstream>

using namespace cv;
using namespace std;

void _showImage(const Mat& image)
{
    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Display window", image); // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window
}

Mat readImage(string image_file)
{
    Mat image = imread(image_file, IMREAD_COLOR); // Read the file

    if (image.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << endl;
        exit(-1);
    }

    return image;
}

int main()
{
    Mat image = readImage("image.jpg"); //input image

    Mat output = image.clone(); // clone for bilateral

    /*
        CODE HERE
    bilateralFilter(image, output, 11, 260, 260);
    */

    imwrite("output.jpg", image); // save image to drive. name of file : output.jpg

    return 0; // run program using local windows debugger
}