#pragma once
#include <windows.h>
#include <shobjidl.h> 
#include <iostream>
#include <bitset>
namespace Validation {

    PWSTR SelectFile();
    PWSTR GetFileName(HRESULT& hr, IShellItem* pItem, PWSTR& FilePath);
    std::string filename_to_string(PWSTR str);
    bool check_filesize(PWSTR cover, PWSTR secret, char path[]);
    void prep_bits(PWSTR cover, PWSTR secret, char path[]);
    template <size_t N>
    std::bitset<N> create_bitset(int bits);
   
}