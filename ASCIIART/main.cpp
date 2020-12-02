#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>
#include <conio.h>

using namespace cv;
using namespace std;

void anyKeyToExit()
{
    cout << "Press Any Key To Exit...";
    _getch();
}

Mat readImage(string image_file)
{
    Mat image = imread(image_file, IMREAD_GRAYSCALE); // Read the file

    if (image.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << endl;
        anyKeyToExit();
        exit(-1);
    }

    return image;
}

string getImageFile() // input and parse image path
{
    string image_name = "";
    cout << "Input Image Path: "; getline(cin, image_name);

    if (image_name.substr(0, 1) == "\"" && image_name.substr(image_name.length() - 1, 1) == "\"")
    {
        image_name = image_name.substr(1, image_name.length() - 2);
    }

    return image_name;
}

void resizeImage(Mat& image, int size, double y_shrink)
{
    double aspect_ratio = 0;
    aspect_ratio = image.size[1] / (double)image.size[0];
    //cout << "Aspect Ratio" << aspect_ratio << endl;

    if (image.size[1] > size)
    {
        int height = (int)round((size / aspect_ratio) / y_shrink);
        resize(image, image, Size(size, height), INTER_LANCZOS4);
    }
}

void normalizeImage(Mat &image, const int &low, const int &high)
{
    for (int y = 0; y < image.size[0]; y++)
    {
        for (int x = 0; x < image.size[1]; x++)
        {
            if ((int)image.at<uchar>(Point(x, y)) < low)
                image.at<uchar>(Point(x, y)) = low;
            else if ((int)image.at<uchar>(Point(x, y)) > high)
                image.at<uchar>(Point(x, y)) = high;
        }
    }

    normalize(image, image, 0, 255, NORM_MINMAX);
}

string convertToASCII(const Mat& image, const char chars[],const int &CHARS_SIZE)
{
    int lum = 0;
    string ascii = "";
    for (int y = 0; y < image.size[0]; y++)
    {
        for (int x = 0; x < image.size[1]; x++)
        {
            lum = image.at<uchar>(Point(x, y));

            ascii += chars[(int)round(lum / 255.0 * ((double)CHARS_SIZE - 1))];
        }
        ascii += '\n';
    }

    return ascii;
}

void _showImage(const Mat& image)
{
    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Display window", image); // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window
}

int main()
{
    Mat image;
    const int CHARS_SIZE = 10; // `~!sTomN@
    char chars[CHARS_SIZE] = { ' ','`','~', '!', 's', 'T', 'o', 'm', 'N' , '@' };
    string ascii = "";
    int size = 110; // width
    double y_shrink = 1.956;
    int low = 60;
    int high = 245;
    string image_file;

    image_file = getImageFile();

    image = readImage(image_file);

    resizeImage(image, size, y_shrink);

    normalizeImage(image, low, high);

    ascii = convertToASCII(image, chars, CHARS_SIZE);

    cout << ascii << endl; // print ascii on console

    anyKeyToExit();

    return 0;
}