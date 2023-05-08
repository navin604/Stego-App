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

}


