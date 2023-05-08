#pragma once
#include <vector>
#include <windows.h>
#include <iostream>
#include <bitset>
namespace Stego {
	void embed(std::vector<char> bits, PWSTR cover_file, char path[]);
}
