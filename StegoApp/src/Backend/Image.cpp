#include "Image.h"

int Image::getBits(PWSTR file)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, file, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, file, -1, &filename[0], len, nullptr, nullptr);
    
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

int Image::getWidth(PWSTR file)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, file, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, file, -1, &filename[0], len, nullptr, nullptr);

    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
   
    return  image.cols;

}

int Image::getHeight(PWSTR file)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, file, -1, nullptr, 0, nullptr, nullptr);
    std::string filename(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, file, -1, &filename[0], len, nullptr, nullptr);
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    return image.rows;

}

void Image::save_image(char path[], cv::Mat img)
{
    cv::imwrite(path, img);
}

cv::Mat Image::open_image(char path[])
{
    cv::Mat stego_image = cv::imread(path, cv::IMREAD_COLOR);
    return stego_image;
}

cv::Mat Image::create_image(int height, int width)
{
    cv::Mat decoded_image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    return decoded_image;
}

