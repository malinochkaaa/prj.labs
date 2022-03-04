#include <opencv2/opencv.hpp>
#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

cv::Mat blue_jpeg, red_jpeg, green_jpeg;

void my_merge(cv::Mat image, cv::Mat rgbchannels[3]) {
	Mat g;
	g = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
	vector<Mat> channels;
	for(int i = 0; i < 3; ++i) {
		if (i == 0) {
			channels.push_back(rgbchannels[i]);
			channels.push_back(g);
			channels.push_back(g);
			cv::merge(channels, blue_jpeg);
			//namedWindow("Blue", 1); 
			//imshow("Blue", blue_jpeg);
		}
		else if(i == 1) {
			channels.push_back(g);
			channels.push_back(rgbchannels[i]);
			channels.push_back(g);
			cv::merge(channels, green_jpeg);
			//namedWindow("Green", 1);
			//imshow("Green", green_jpeg);
		}
		else {
			channels.push_back(g);
			channels.push_back(g);
			channels.push_back(rgbchannels[i]);
			cv::merge(channels, red_jpeg);
			//namedWindow("Red", 1);
			//imshow("Red", red_jpeg);
		}
		channels.clear();
	}
}

cv::Mat mosaic(cv::Mat img) {
	Mat BGRchannels[3];
	cv::split(img, BGRchannels);
	my_merge(img, BGRchannels);

	cv::Mat merged_image = cv::Mat::zeros(img.rows * 2, img.cols * 2, CV_8UC3);
	img.copyTo(merged_image(cv::Rect(0, 0, img.cols, img.cols)));
	green_jpeg.copyTo(merged_image(cv::Rect(0, img.rows, img.cols, img.cols)));
	red_jpeg.copyTo(merged_image(cv::Rect(img.rows, 0, img.cols, img.cols)));
	blue_jpeg.copyTo(merged_image(cv::Rect(img.rows, img.rows, img.cols, img.cols)));
	return merged_image;
}
cv::Mat histogram(Mat image) {
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;

	Mat BGRchannels[3];
	cv::split(image, BGRchannels);
	Mat b_hist, g_hist, r_hist;
	calcHist(&BGRchannels[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&BGRchannels[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&BGRchannels[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);

	int hist_w = 512, hist_h = 256;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	return histImage;
}

int main() {
	std::string image_path = ("../../../data/cross_0256x0256.png");
	cv::Mat img = imread(image_path, IMREAD_COLOR);
	if (img.empty()) {
		std::cout << "Image not found" << std::endl;
	}

	vector<int> compression;
	compression.push_back(IMWRITE_JPEG_QUALITY);
	compression.push_back(25);

	imwrite("cross_0256x0256_025.jpg", img, compression);
	Mat img_jpeg = imread("cross_0256x0256_025.jpg");;

	cv::Mat merged_img_png = mosaic(img);
	imwrite("cross_0256x0256_png_channels.png", merged_img_png);

	cv::Mat merged_img_jpeg = mosaic(img_jpeg);
	imwrite("cross_0256x0256_jpg_channels.png", merged_img_jpeg);

	cv::Mat hist_img_png = histogram(img);
	cv::Mat hist_img_jpg = histogram(img_jpeg);

	cv::Mat hist_merged = cv::Mat::zeros(hist_img_png.cols, hist_img_png.cols, CV_8UC3);
	hist_img_png.copyTo(hist_merged(cv::Rect(0, 0, hist_img_png.cols, hist_img_png.rows)));
	hist_img_jpg.copyTo(hist_merged(cv::Rect(0, hist_img_png.rows, hist_img_png.cols, hist_img_png.rows)));
	imwrite("cross_0256x0256_hists.png", hist_merged);

	waitKey();
}