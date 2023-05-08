#include "Image.h"

int Image::getBits(PWSTR img)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, img, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, img, -1, &filename[0], len, nullptr, nullptr);
    
	cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    int pixels = image.rows * image.cols;
    return pixels;
    
}

cv::Mat Image::getImageObj(PWSTR file)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, file, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, file, -1, &filename[0], len, nullptr, nullptr);
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    return image;
}

int Image::getWidth(PWSTR img)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, img, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, img, -1, &filename[0], len, nullptr, nullptr);

    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
   
    return  image.cols;

}

int Image::getHeight(PWSTR img)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, img, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, img, -1, &filename[0], len, nullptr, nullptr);

    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    
    return image.rows;

}

