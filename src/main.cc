// Binarize an image.
// Zexi Mao, Apr. 2015

#include <cstdlib>
#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

#include "binarize_utils.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " input_path output_path" << endl;
        exit(-1);
    }

    string input_path(argv[1]);
    string output_path(argv[2]);

    IplImage* img = cvLoadImage(input_path.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    int threshold = ComputeThreshold(img);
    IplImage* bin_img = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
    cvThreshold(img, bin_img, (double)threshold, 255, CV_THRESH_BINARY_INV);
    cvSaveImage(output_path.c_str(), bin_img);

    return 0;
}

