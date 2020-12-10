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

enum class parsing { SUCCESS, FAIL, PROMPT };

void anyKeyToExit();
void printUserInput(int argc, char* argv[]);
string cleanImagePath(string image_file);
void printHelp();
void promptUserForInput(string& image_file, int& size, int& low, int& high, bool& black_bg, string& text_file);
parsing parseArgs(int argc, char* argv[]);
Mat readImage(string image_file);
void resizeImage(Mat& image, int size, double y_shrink);
void normalizeImage(Mat& image, const int& low, const int& high);
void deNoise(Mat& image);
string convertToASCII(const Mat& image, const char chars[], const int& CHARS_SIZE);
void saveAscii(const string& ascii, const string& text_file);
void invertChars(char chars[], const int& CHAR_SIZE);

int main(int argc, char* argv[])
{
    Mat image;
    const int CHARS_SIZE = 10; // `~!sTomN@
    char chars[CHARS_SIZE] = { ' ','`', '~', '!', 's', 'T', 'o', 'm', 'N' , '@' };
    double y_shrink = 1.956;
    string image_file;
    int size = 0;
    int low = 0;
    int high = 0;
    bool black_bg = false;
    string text_file = "";
    bool prompt_used = false;
    string ascii = "";

    const parsing result = parseArgs(argc, argv);
    if (result == parsing::SUCCESS)
    {
        image_file = (string)argv[1];
        size = atoi(argv[2]);
        low = atoi(argv[3]);
        high = atoi(argv[4]);
        black_bg = (bool)atoi(argv[5]);
        text_file = (string)argv[6];
    }
    else if (result == parsing::PROMPT)
    {
        promptUserForInput(image_file, size, low, high, black_bg, text_file);
        prompt_used = true;
    }
    else
    {
        return 0;
    }

    if (!black_bg)
    {
        invertChars(chars, CHARS_SIZE);
    }

    image = readImage(image_file);
    if (image.empty())
    {
        cout << "Failed to open specified image file" << endl;
        if (prompt_used)
        {
            anyKeyToExit();
        }
        else
        {
            return 0;
        }
    }

    resizeImage(image, size, y_shrink);

    normalizeImage(image, low, high);

    ascii = convertToASCII(image, chars, CHARS_SIZE);

    cout << ascii; // print ascii on console

    saveAscii(ascii, text_file);

    if (prompt_used)
    {
        anyKeyToExit();
    }

    return 0;
}

/*
Press any key to end the program
*/
void anyKeyToExit()
{
    cout << "Press Any Key To Exit...";
    char _ = _getch();
    exit(0);
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
Input and clean double quotes ("") from file path
@return file path
*/
string cleanImagePath(string image_file)
{
    if (image_file.substr(0, 1) == "\"" && image_file.substr(image_file.length() - 1, 1) == "\"")
    {
        image_file = image_file.substr(1, image_file.length() - 2);
    }
    return image_file;
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
    cout << setw(20) << "black_bg" << '\t' << "whether to have the background of converted image be black or not (0-1)(0 == false; 1 == true)" << endl;
    cout << setw(20) << "text file path" << '\t' << "path to the text file where the converted image will be saved" << endl << endl;
    cout << "Example: ./myimage.jpg 110 40 240 1 art.txt" << endl;
}

/*
Prompt user for input
*/
void promptUserForInput(string& image_file, int& size, int& low, int& high, bool& black_bg, string& text_file)
{
    cout << "Input image file: "; getline(cin, image_file); image_file = cleanImagePath(image_file);

    do
    {
        cout << "Input Size (>0): "; cin >> size;
    } while (size < 1);

    do
    {
        cout << "Input lower value for normalization (0-255): "; cin >> low;
    } while (low < 1 && low > 255);

    do
    {
        cout << "Input upper value for normalization (0-255): "; cin >> high;
    } while (high < 1 && high > 255);

    cout << "Produce image with black background config (0 = no | !0 = yes): "; cin >> black_bg;

    cin.ignore(1000, '\n');
    cout << "Input text file path for output: "; getline(cin, text_file); text_file = cleanImagePath(text_file);
}

/*
Validates the input arguments from the user
@param argc: size of input argument array
@param argv: input argument array
@return true if successful and false otherwise
*/
parsing parseArgs(int argc, char* argv[])
{
    if (argc == 7 && atoi(argv[2]) > 0 && atoi(argv[3]) >= 0 && atoi(argv[3]) < 255 && atoi(argv[4]) > 0 && atoi(argv[4]) <= 255 && atoi(argv[5]) >= 0 && atoi(argv[5]) <= 1)
    {
        cout << "Parse Successful..." << endl;
        return parsing::SUCCESS;
    }
    else if (argc == 2 && ((string)argv[1] == "-h" || (string)argv[1] == "--help"))
    {
        printHelp();
        return parsing::FAIL;
    }
    else if (argc == 1)
    {
        return parsing::PROMPT;
    }
    else
    {
        cout << "Parse Unsuccessful..." << endl;

        printHelp();

        printUserInput(argc, argv);
        return parsing::FAIL;
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

    return image;
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
            { 
                image.at<uchar>(Point(x, y)) = low;
            }
            else if ((int)image.at<uchar>(Point(x, y)) > high)
            { 
                image.at<uchar>(Point(x, y)) = high;
            }
        }
    }

    normalize(image, image, 0, 255, NORM_MINMAX);
}

/*
Denoise the image using the Bilateral Filter
@param image: cv2 image array
*/
void deNoise(Mat& image)
{
    Mat out = image.clone();
    bilateralFilter(image, out, 2, 60, 60);
    image = out.clone();
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

//int size = 110; // width
//int low = 60;
//int high = 245;
//bool black_bg = false;
//string image_file "image.jpg";
//string text_file = "ASCII.txt";

/*
CS 100 Project
By:
Muhammad Hamza 24100192
Imaan Hameed 24100093
Resources Used: - https://docs.opencv.org/master/d9/df8/tutorial_root.html
*/