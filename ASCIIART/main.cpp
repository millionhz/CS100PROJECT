#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <fstream>
#include <iomanip>

using namespace cv;
using namespace std;



/*
Press any key to end the program
*/
void anyKeyToExit()
{
    cout << "Press Any Key To Exit...";
    char _ = _getch();
    exit(-1);
}

/*
Prints user input onto the console
@param argc: size of input argument array
@param argv: input argument array
*/
void printUserInput(int argc, char* argv[])
{
    cout << "Your Input: ";
    for (int i = 1; i < argc; i++)
    {
        cout << argv[i] << " ";
    }
    cout << "\b " << endl;
}


/*
Prints help onto the console
*/
void printHelp()
{
    cout << "Usage: [image file path] [size] [low] [high] [black_bg] [text file path]" << endl << endl;
    cout << setw(20) << "image file path" << '\t' << "path to the image file you want to convert" << endl;
    cout << setw(20) << "size" << '\t' << "number of characters in a row of the converted image (higher == more detail)" << endl;
    cout << setw(20) << "low" << '\t' << "lower pixel value for normalizatiBon (0-255)" << endl;
    cout << setw(20) << "high" << '\t' << "higher pixel value for normalization (0-255)" << endl;
    cout << setw(20) << "black_bg" << '\t' << "whether to have the backgound of convertted image be black or not (0-1)(0 == false; 1 == true)" << endl;
    cout << setw(20) << "text file path" << '\t' << "path to the text file where the converted image will be saved" << endl << endl;
    cout << "Example: ./myimage.jpg 110 40 240 1 art.txt" << endl;
}

/*
Validates the input arguments from the user
@param argc: size of input argument array
@param argv: input argument array
@return true if successful and false otherwise
*/
bool parseArgs(int argc, char* argv[])
{
    if (argc == 7 && atoi(argv[2]) > 0 && atoi(argv[3]) >= 0 && atoi(argv[3]) < 255  && atoi(argv[4]) > 0 && atoi(argv[4]) <= 255 && atoi(argv[5]) >= 0 && atoi(argv[5]) <= 1)

    {
        cout << "Parse Successful..." << endl;
        return true;
    }
    else if ( argc == 2 && ((string)argv[1] == "-h" || (string)argv[1] == "--help"))
    {
        printHelp();
        return true;
    }
    else if (argc == 1)
    {
        printHelp();
        return false;;

    }
    else
    {
        cout << "Parse Unsuccessful..." << endl;

        printHelp();

        printUserInput(argc, argv);
        return false;
    }
}

/*
Loads image file form disk
@param image_file: path to the image file
@return cv2 iamge array
*/
Mat readImage(string image_file)
{
    Mat image = imread(image_file, IMREAD_GRAYSCALE); // Read the file

    if (image.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << endl;
        anyKeyToExit();
    }

    return image;
}

/*
Input and clean double quotes ("") from file path
@return file path
*/
string getImageFile()
{
    string image_name = "";
    cout << "Input Image Path: "; getline(cin, image_name);

    if (image_name.substr(0, 1) == "\"" && image_name.substr(image_name.length() - 1, 1) == "\"")
    {
        image_name = image_name.substr(1, image_name.length() - 2);
    }

    return image_name;
}


/*
Resizes the image while maintaining aspect ratio
@param image: cv2 image array
@param size: number of characters in a line
@param y_shrink: vertical shrink constant
*/
void resizeImage(Mat& image, int size, double y_shrink)
{
    double aspect_ratio = 0;
    aspect_ratio = image.size[1] / (double)image.size[0];
    //image.size[1] = x-axis dim
    int height = (int)round((size / aspect_ratio) / y_shrink);
    resize(image, image, Size(size, height), INTER_LANCZOS4);

}

/*
Normalizes the image (contrast)
@param image : cv2 image array
@param low : low value (0-255) of the normalization
@param high : high value (0-255) of the normalization
*/
void normalizeImage(Mat& image, const int& low, const int& high)
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

/*
processes image to produce ASCII
@param image : cv2 image array
@param chars : array of characters used for processing
@param CHARS_SIZE : size of the array
@return string of ascii characters
*/
string convertToASCII(const Mat& image, const char chars[], const int& CHARS_SIZE)
{
    int lum = 0; // luminance
    string ascii = "";
    for (int y = 0; y < image.size[0]; y++)
    {
        for (int x = 0; x < image.size[1]; x++)
        {
            lum = image.at<uchar>(Point(x, y)); // pixel brightness value
            ascii += chars[(int)round(lum / 255.0 * ((double)CHARS_SIZE - 1))]; // chars[0] (int)round(lum / 255.0 * ((double)CHARS_SIZE - 1))
        }
        ascii += '\n';
    }

    return ascii;
}

/*
Saves string as a text file
@param ascii: ASCII string generated
@param text_file: path of the output text file
*/
void saveAscii(const string& ascii, const string& text_file)
{
    ofstream out_file;
    out_file.open(text_file);
    out_file << ascii;
}

/*
Inverts the array of characters
@param chars : array containing characters
@param CHARS_SIZE : size of the array
*/
void invertChars(char chars[], const int& CHAR_SIZE)
{
    for (int low = 0, high = CHAR_SIZE - 1; low < high; low++, high--)
    {
        swap(chars[low], chars[high]);
    }
}

//void _showImage(const Mat& image)
//{
//    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
//    imshow("Display window", image); // Show our image inside it.
//    waitKey(0); // Wait for a keystroke in the window
//}

int main(int argc, char* argv[])
{

    if (!parseArgs(argc, argv))
    {
        return 0;
    }

    Mat image;
    const int CHARS_SIZE = 10; // `~!sTomN@
    char chars[CHARS_SIZE] = { ' ','`','~', '!', 's', 'T', 'o', 'm', 'N' , '@' };
    double y_shrink = 1.956;

    //int size = 110; // width
    //int low = 60;
    //int high = 245;
    //bool black_bg = false;
    //string image_file "image.jpg";
    //string text_file = "ASCII.txt";

    string image_file = (string)argv[1];
    int size = atoi(argv[2]);
    int low = atoi(argv[3]);
    int high = atoi(argv[4]);
    bool black_bg = (bool)atoi(argv[5]);
    string text_file = (string)argv[6];

    string ascii = "";

    if (!black_bg)
    {
        invertChars(chars, CHARS_SIZE);
    }

    //image_file = getImageFile();

    image = readImage(image_file);

    resizeImage(image, size, y_shrink);

    normalizeImage(image, low, high);

    ascii = convertToASCII(image, chars, CHARS_SIZE);

    cout << ascii; // print ascii on console

    saveAscii(ascii, text_file);

    //anyKeyToExit();

    return 0;
}