#pragma once

#include "Validation.h"
#include <windows.h>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// Crypto++ files
#include <string>
#include "cryptlib.h"
#include "filters.h"
#include "aes.h"
#include "sha.h"

#include "base64.h"
#include "osrng.h"
#include "files.h"
#include "pwdbased.h"
#include "modes.h"
#include "hex.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <bitset>
#include "cryptlib.h"
#include "filters.h"
#include <vector>


namespace Image {
	int getBits(PWSTR file);
	int getWidth(PWSTR file);
	int getHeight(PWSTR file);
	cv::Mat getImageObj(PWSTR file);
	void save_image(char path[], cv::Mat img);
	cv::Mat open_image(char path[]);
	std::vector<bool> encrypt_image(PWSTR file, char pass[]);
	std::string decrypt_image(std::string& message_strings, char password[]);
	std::vector<bool> int_to_bits(std::string& message_strings);
}
