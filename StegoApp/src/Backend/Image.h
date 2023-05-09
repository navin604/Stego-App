#pragma once
#include <windows.h>#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


namespace Image {
	int getBits(PWSTR file);
	int getWidth(PWSTR file);
	int getHeight(PWSTR file);
	cv::Mat getImageObj(PWSTR file);
	void save_image(char path[], cv::Mat img);
	cv::Mat open_image(char path[]);
	cv::Mat create_image(int height, int width);
}
