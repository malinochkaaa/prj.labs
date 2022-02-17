#include <opencv2/opencv.hpp>

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
	cv::Mat I = cv::Mat::zeros(180, 768, CV_8UC1);

	for (int i = 0; i < I.rows; i++)
	{
		for (int j = 0; j < I.cols; j++)
		{
			I.at<uint8_t>(i, j) += j / 3;
		}
	} 

	cv::Mat I_pow = pow_gamma(I);
	cv::Mat I_pixel = pixel_gamma(I);

	int64 t0 = cv::getTickCount();
	cv::Mat main_image = cv::Mat::zeros(I.rows * 3, I.cols, CV_8UC1);
	double t1 = (double)cv::getTickCount();
	I.copyTo(main_image(cv::Rect(0, I.rows * 0, I.cols, I.rows)));
	double t2 = (double)cv::getTickCount();
	I_pow.copyTo(main_image(cv::Rect(0, I.rows * 1, I.cols, I.rows)));
	double t3 = (double)cv::getTickCount();
	I_pixel.copyTo(main_image(cv::Rect(0, I.rows * 2, I.cols, I.rows)));


	double r1 = 1000 * ((t1 - t0) / cv::getTickFrequency());
	double r2 = 1000 * ((t2 - t1) / cv::getTickFrequency());
	double r3 = 1000 * ((t3 - t2) / cv::getTickFrequency());

	std::cout << "Gradient: " << std::setprecision(2) << r1 << " ms \n" << "Pow gamma: " << std::setprecision(2) << r2 << " ms \n"
		<< "Pixel gamma: " << std::setprecision(2) << r3 << " ms " << std::endl;

	imwrite("lab01.png", main_image);
	imshow("lab01.png", main_image);
	cv::waitKey(0);
}
