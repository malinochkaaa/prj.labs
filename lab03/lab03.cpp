#include <opencv2/opencv.hpp>
#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

float my_function(float x) {
    return 20.0*sqrt(x);
}

vector <uchar> altered() {
    vector<uchar> my_lut(256);
    for (int i = 0; i < 256; ++i) {
        my_lut[i] = my_function(i);
    }
    return my_lut;
}

int main() {
    std::string image_path = ("./data/cross_0256x0256.png");
    Mat img = imread(image_path, IMREAD_COLOR);
    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    Mat visual(512, 512, CV_8UC1, 255);
    line(visual, Point(0, 511), Point(0, 0), Scalar(128, 128, 128), 2);
    line(visual, Point(511, 0), Point(0, 0), Scalar(128, 128, 128), 2);
    for (int i(0); i < visual.cols; ++i) {
        circle(visual, Point(i, 511 - my_function(i)), 1, Scalar(0), FILLED);
    }
    imwrite("lab03_viz_func.png", visual);

    Mat gre;
    cvtColor(img, gre, COLOR_BGR2GRAY);

    Mat rgb_res, gre_res;
    LUT(img, altered(), rgb_res);
    LUT(gre, altered(), gre_res);

    imwrite("lab03_rgb.png", img);
    imwrite("lab03_gre.png", gre);
    imwrite("lab03_rgb_res.png", rgb_res);
    imwrite("lab03_gre_res.png", gre_res);

    waitKey(0);
    return 0;
}