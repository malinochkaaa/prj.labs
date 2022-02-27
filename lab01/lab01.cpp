#include <opencv2/opencv.hpp>
#include <chrono>


const float gamma = 2.2;

cv::Mat pow_gamma(cv::Mat img) 
{
	img.convertTo(img, CV_64F, 1 / 255.0);
	cv::pow(img, gamma, img);
	img.convertTo(img, CV_8UC1, 255.0);
	return img;
}

cv::Mat pixel_gamma(cv::Mat img) 
{
	cv::Mat img_float;
	img.convertTo(img_float, CV_64F);
	for (int i = 0; i < img_float.rows; ++i)
		for (int j = 0; j < img_float.cols; ++j)
		{
			img_float.at<double>(i, j) = (pow(img_float.at<double>(i, j) / 255.0, gamma) * 255.0);
		}
	img_float.convertTo(img_float, CV_8UC1);
	return img_float;
	
}

int main() 
{
	cv::Mat I = cv::Mat::zeros(60, 768, CV_8UC1);

	for (int i = 0; i < I.rows; i++)
	{
		for (int j = 0; j < I.cols; j++)
		{
			I.at<uint8_t>(i, j) += j / 3;
		}
	} 

	auto start = std::chrono::steady_clock::now();
	cv::Mat I_pow = pow_gamma(I);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	start = std::chrono::steady_clock::now();
	cv::Mat I_pixel = pixel_gamma(I);
	end = std::chrono::steady_clock::now();
    elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	cv::Mat main_image = cv::Mat::zeros(I.rows * 3, I.cols, CV_8UC1);
	I.copyTo(main_image(cv::Rect(0, I.rows * 0, I.cols, I.rows)));
	I_pow.copyTo(main_image(cv::Rect(0, I.rows * 1, I.cols, I.rows)));
	I_pixel.copyTo(main_image(cv::Rect(0, I.rows * 2, I.cols, I.rows)));


	imwrite("lab01.png", main_image);
	imshow("lab01.png", main_image);
	cv::waitKey(0);
}
