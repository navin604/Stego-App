#pragma once
#include <vector>
#include <windows.h>
#include <iostream>
#include <bitset>
#include <map>

namespace Stego {
	void embed(std::vector<char> bits, PWSTR cover_file, char path[]);
	int decode( char path[], char password[]);
	void process_bit(int& current_bit, int lsb, std::string& current_type, std::map<std::string, std::string>& message_strings, bool& complete, std::map<std::string, int>& message_types);
	void change_type(std::string& current_type, std::map<std::string, std::string>& message_strings, std::map<std::string, int>& message_types, bool& complete);
	void set_file_name(std::map<std::string, std::string>& message_strings);
}
