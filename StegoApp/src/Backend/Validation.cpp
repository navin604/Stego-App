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


    void prep_bits(PWSTR cover, PWSTR secret, char path[])
    {
        // Vec will contain all bits to be embedded
        std::vector<char> vec;
        //Get bits in secret image
        int secret_bits;
        secret_bits = Image::getBits(secret) * 24;
        int n = 32;
        std::bitset<32> secret_bitset = create_bitset<32>(secret_bits);
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






    }

    template <size_t N>
    std::bitset<N> create_bitset(int bits)
    {
        return std::bitset<N> (bits);
       
    }



}


