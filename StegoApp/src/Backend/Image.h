#pragma once
#include <windows.h>#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


namespace Image {
	int getBits(PWSTR img);
	int getWidth(PWSTR img);
	int getHeight(PWSTR img);
}
