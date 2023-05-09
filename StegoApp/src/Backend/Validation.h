#pragma once
#include <windows.h>
#include <shobjidl.h> 
#include <iostream>
#include <bitset>
#include <vector>

namespace Validation {

    PWSTR SelectFile();
    PWSTR GetFileName(HRESULT& hr, IShellItem* pItem, PWSTR& FilePath);
    std::string filename_to_string(PWSTR str);
    bool check_filesize(PWSTR cover, PWSTR secret, char path[]);
    std::vector<char> prep_bits(PWSTR cover, PWSTR secret, char path[]);
    int bin_to_int(std::string str);
    
   
   
}