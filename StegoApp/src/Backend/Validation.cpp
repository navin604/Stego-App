#include "Validation.h"

#include "Image.h"



namespace Validation {

    std::string filename_to_string(PWSTR str)
    {
        // Converts PWSTR from dialog object to std::string
        int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
        if (len == 0) {
            return "";
        }

        std::string result(len - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, str, -1, &result[0], len, NULL, NULL);

        return result;
    }



    bool check_filesize(PWSTR cover, PWSTR secret, char path[], char pass[])
    {
        int bits_to_embed = 32 + 32;
        int cover_bits;
        int secret_bits;
        std::string filename = filename_to_string(secret);
        //Calculate available bits in cover image
        cover_bits = Image::getBits(cover) * 3;
        std::cout << "Cover IMage: " << cover_bits << "\n";

        // Calculate bits required to hold encrypted secret image
        secret_bits = Image::encrypt_image(secret, pass).size();
        std::cout << "Secret image encrypted bit size: " << secret_bits << "\n";
        std::cout << "filename in bits: " << filename.size() * 8 << "\n";
        std::cout << "filename " << filename << "\n";

        bits_to_embed = bits_to_embed + secret_bits + filename.size() * 8;
        std::cout << "Estimate embedding: " << bits_to_embed << "\n";
        if (bits_to_embed < cover_bits)
        {
            return true;
        }
        return false;
    }
    
   
    std::string get_filename(std::string s)
    {
        // Extract filename from filepath
        std::string name = "";
        for (int i = s.size()-1; i >= 0; i--)
        {
            if (s[i] == '/' or s[i] == '\\')
            {
                break;
            }
            name.push_back(s[i]);
        }
        int n = name.length();

        // Reverse string
        for (int i = 0; i < n / 2; i++)
            std::swap(name[i], name[n - i - 1]);

        return name;
    }
    std::vector<char> prep_bits(PWSTR cover, PWSTR secret, char path[], char pass[])
    {     
        
        std::vector<char> vec;
        
        std::string secret_file_path_str = filename_to_string(secret);
        std::string filename = get_filename(secret_file_path_str);
       
        std::vector<bool> secret_bits = Image::encrypt_image(secret, pass);
        int secret_bits_count = secret_bits.size();

        
        int n = 32;
        std::bitset<32> secret_bitset(secret_bits_count);
        std::string str_total = secret_bitset.to_string();

        // Add total message size to vector
        for (int i = 0; i < str_total.length(); i++)
        {
            vec.push_back(str_total[i]);
        }

        std::string binary_string;

        // Convert each char in file name to binary equiv
        for (int i = 0; i < filename.length(); i++)
        {
            std::bitset<8> byte(filename[i]);
            binary_string += byte.to_string();
        }


        std::bitset<32> filename_bitset(binary_string.length());
        std::string filename_bitset_str = filename_bitset.to_string();
       
        // Add amount of bits in filename
        for (auto c : filename_bitset_str)
        {
            vec.push_back(c);
        }
        
        // Add file name bitstring
        for (auto c : binary_string)
        {
            vec.push_back(c);
        }
       

        int check = vec.size();
        for (const auto& elem : secret_bits) {
            vec.push_back(elem ? '1' : '0');
            check++;
        }
        
        return vec;
    }


    PWSTR SelectFile()
	{
        // This function handles the file selection for the GUI - Uses windows API
        // Taken from docs -> https://learn.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box

        PWSTR FilePath = nullptr;
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
            COINIT_DISABLE_OLE1DDE);
        
        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Creates file dialog object
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Opens file dialog
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    FilePath = GetFileName(hr, pItem, FilePath);
                    pItem->Release();
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
        return FilePath;
	}



	PWSTR GetFileName(HRESULT& hr, IShellItem* pItem, PWSTR& FilePath)
	{
        // This function retrieves file name from dialog object
        // Taken from docs -> https://learn.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box

        if (SUCCEEDED(hr))
        {
            // Extract file name
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &FilePath);

            // Return file name if operation succeeded.
            if (SUCCEEDED(hr))
            {
                return FilePath;
                
            }
            
        }
	}

}


   