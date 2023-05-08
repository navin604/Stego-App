#include "Validation.h"

#include "Image.h"



namespace Validation {

    PWSTR SelectFile()
	{
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
    std::string filename_to_string(PWSTR str)
    {
        int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
        if (len == 0) {
            return "";
        }

        std::string result(len - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, str, -1, &result[0], len, NULL, NULL);

        return result;
    }
    bool check_filesize(PWSTR cover, PWSTR secret, char path[])
    {
        int bits_to_embed = 32 + 13 + 13 + 32;
        int cover_bits;
        int secret_bits;
        cover_bits = Image::getBits(cover) * 3;
        
        secret_bits = Image::getBits(secret);
        bits_to_embed = bits_to_embed + (secret_bits * 24) + (strlen(path)*8);
        if (bits_to_embed < cover_bits)
        {
            return true;
        }
        return false;
    }


    std::vector<char> prep_bits(PWSTR cover, PWSTR secret, char path[])
    {
        // Vec will contain all bits to be embedded
        std::vector<char> vec;
        //Get bits in secret image
        int secret_bits;
        secret_bits = Image::getBits(secret) * 24;
        int n = 32;
        std::bitset<32> secret_bitset(secret_bits);
        std::string str_total = secret_bitset.to_string();

        // Add total message size to vector
        for (int i = 0; i < str_total.length(); i++)
        {
            vec.push_back(str_total[i]);
        }
        /*for (int i = 0; i < vec.size(); i++)
        {
            std::cout << vec[i] << "\n";
        }*/


        int width = Image::getWidth(secret);
        int height = Image::getHeight(secret);
        std::bitset<13> width_b(width);
        std::string width_b_str = width_b.to_string();
        std::bitset<13> height_b(height);
        std::string height_b_str = height_b.to_string();



        // Add width to vector
        for (int i = 0; i < width_b_str.length(); i++)
        {
            vec.push_back(width_b_str[i]);
        }
        // Add height to vector
        for (int i = 0; i < height_b_str.length(); i++)
        {
            vec.push_back(height_b_str[i]);
        }


        std::string binary_string;
        
        // Convert each char in file name to binary equiv
        for (int i = 0; i < strlen(path); i++)
        {
            std::bitset<8> byte(path[i]);
            binary_string += byte.to_string();
        }

        std::bitset<32> filename_bitset(binary_string.length());

        std::string filename_bitset_str = filename_bitset.to_string();
        // Add length of file name 
        for (auto c : filename_bitset_str)
        {
            vec.push_back(c);
        }

        // Add file name to vec
        for (auto c : binary_string)
        {
            vec.push_back(c);
        }
        cv::Mat image = Image::getImageObj(secret);

        //Add image to vec
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                cv::Vec3b Pixel = image.at<cv::Vec3b>(i, j);
                int r = Pixel[2];
                int g = Pixel[1];
                int b = Pixel[0];
                std::string rb = std::bitset<8>(r).to_string();
                std::string gb = std::bitset<8>(g).to_string();
                std::string bb = std::bitset<8>(b).to_string();
                for (int i = 0; i < 8; i++)
                {
                    vec.push_back(rb[i]);
                }
                for (int i = 0; i < 8; i++)
                {
                    vec.push_back(gb[i]);
                }
                for (int i = 0; i < 8; i++)
                {
                    vec.push_back(bb[i]);
                }
            }
        }



        return vec;
    }

    
}


