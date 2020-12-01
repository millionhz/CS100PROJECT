#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    //if (argc != 2)
    //{
    //    cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
    //    return -1;
    //}

    //Mat image;
    const int CHARS_SIZE = 10; // `~!sTomN@
    char chars[CHARS_SIZE] = { ' ','`','~', '!', 's', 'T', 'o', 'm', 'N' , '@' }; 
    //char chars[CHARS_SIZE] = { '@', 'N', 'm', 'o', 'T', 's', '!', '~', '`', ' ' };

    string text = "";
    double aspect_ratio = 0;
    int size = 110; // width
    double y_shrink = 1.956;
    string image_name;

    cout << "Input Image Path: "; getline(cin, image_name); 
    
    if (image_name.substr(0,1) == "\"" && image_name.substr(image_name.length()-1, 1) == "\"")
    {
        image_name = image_name.substr(1, image_name.length() - 2);

    }
    cout << image_name << endl;
    Mat image = imread(image_name, IMREAD_GRAYSCALE); // Read the file


    if (image.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    aspect_ratio = image.size[1] / (double)image.size[0];
    //cout << "Aspect Ratio" << aspect_ratio << endl;

    if(image.size[1] > size)
    { 
        cout << "Resizing Image" << endl;
        int height = round((size / aspect_ratio) / y_shrink);
        resize(image, image, Size(size, height), INTER_LANCZOS4);
    }


    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    //imshow("Display window", image); // Show our image inside it.
    //waitKey(0); // Wait for a keystroke in the window

    //cout << image.size[0] << endl;
    int lum;
    int char_val;
    for (int y = 0; y < image.size[0]; y++)
    {
        for (int x = 0; x < image.size[1]; x++)
        {
            lum = image.at<uchar>(Point(x, y));
            char_val = round(lum / 255.0 * ((double)CHARS_SIZE -1));
            //cout << char_val << '\t';
            if (char_val >= CHARS_SIZE)
            {
                cout << "Char error..." << endl;
                return -1;
            }
            text += chars[char_val];
        }
        text += '\n';
    }

    cout << text;
    //int _;
    //cin >> _;
    return 0;
}