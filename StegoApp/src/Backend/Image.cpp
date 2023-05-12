#include "Image.h"

int Image::getBits(PWSTR file)
{
    // Retrieves number of pixels in image
    std::string filename = Validation::filename_to_string(file);
    
	cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    int pixels = image.rows * image.cols;
    return pixels;
    
}


std::vector<bool> Image::encrypt_image(PWSTR file, char pass[])
{
    std::string filename = Validation::filename_to_string(file);

    // Open image as vector of bytes
    std::ifstream image_file(filename, std::ios::binary);
    std::vector<CryptoPP::byte> plaintext_image((std::istreambuf_iterator<char>(image_file)), std::istreambuf_iterator<char>());
    image_file.close();
    std::cout << "Secret image without encryption: " << plaintext_image.size() << "\n";
    // Create key and set padding -> https://www.cryptopp.com/wiki/PKCS5_PBKDF2_HMAC
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    std::string password = pass;
    
    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
    pbkdf.DeriveKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, 0x00, (CryptoPP::byte*)password.data(), password.size(), NULL, 0, 1000);

    std::string image_str(plaintext_image.begin(), plaintext_image.end());
    int num_pad = CryptoPP::AES::BLOCKSIZE - (image_str.length() % CryptoPP::AES::BLOCKSIZE);
    image_str.append(num_pad, static_cast<char>(num_pad));

    // Encrypt image
    std::string encrypted_img_str;
    CryptoPP::ECB_Mode< CryptoPP::AES>::Encryption encrypt;
    encrypt.SetKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    // Specify output string -> https://www.cryptopp.com/wiki/StreamTransformationFilter
    CryptoPP::StringSource(image_str, true, new CryptoPP::StreamTransformationFilter(encrypt, new  CryptoPP::StringSink(encrypted_img_str)));

    // Extract bits from bytes
    // Vector of bools used to hold bits
    std::vector<bool> bits;
    for (const auto& byte : encrypted_img_str)
    {

        // Byte to bit -> https://people.duke.edu/~ng46/borland/byte-to-bits.htm
        for (int i = 0; i < 8; i++)
        {
            // Shift and extract bit
            bits.push_back((byte >> i) & 1);
        }
    }
    return bits;
}

std::string Image::decrypt_image(std::string& message_strings, char password[])
{

    std::vector<bool> bits = int_to_bits(message_strings);
    // Convert the ciphertext bits back into a string of bytes
    std::string encrypted_img_str;
    for (std::size_t i = 0; i < bits.size(); i += 8)
    {
        char byte = 0;
        for (int j = 0; j < 8; j++)
        {
            // Shift bits and set value in byte
            byte |= static_cast<char>(bits[i + j]) << j;
        }
        encrypted_img_str += byte;
    }
    
    // Create key and set padding, same as encryption encrypt_image()
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    std::string password_str = password;
    
    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
    pbkdf.DeriveKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, 0x00, (CryptoPP::byte*)password_str.data(), password_str.size(), NULL, 0, 1000);
    
    // Decrypt image
    std::string image_bytes;
    CryptoPP::ECB_Mode< CryptoPP::AES>::Decryption decrypt;
    
    decrypt.SetKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    
    try
    {
        CryptoPP::StringSource(encrypted_img_str, true, new CryptoPP::StreamTransformationFilter(decrypt, new CryptoPP::StringSink(image_bytes)));
    }
    catch (std::exception& ex)
    {
        return "";
    }
    
        
    // Extract password from padded data
    std::size_t num_pad = static_cast<std::size_t>(image_bytes.back());
    image_bytes.erase(image_bytes.end() - num_pad, image_bytes.end());

    return image_bytes;    
}




std::vector<bool> Image::int_to_bits(std::string& message_strings)
{
    // Convert bit string into vector of bools to be decrypted
    std::string encrypted_img_str = message_strings;
    std::vector<bool> bits;
    for (char bit : encrypted_img_str)
    {
       bits.push_back(bit == '1');
        
    }
    return bits;
}



cv::Mat Image::getImageObj(PWSTR file)
{
    // Reads in image as OpenCV object from windows.h obj
    std::string filename = Validation::filename_to_string(file);
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    return image;
}

int Image::getWidth(PWSTR file)
{
    // Returns width of image
    std::string filename = Validation::filename_to_string(file);

    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
   
    return  image.cols;

}

int Image::getHeight(PWSTR file)
{
    // Returns height of image
    std::string filename = Validation::filename_to_string(file);
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    return image.rows;

}

void Image::save_image(char path[], cv::Mat img)
{
    // Saves image to specified path
    cv::imwrite(path, img);
}

cv::Mat Image::open_image(char path[])
{
    // Reads in image from user input
    cv::Mat stego_image = cv::imread(path, cv::IMREAD_COLOR);
    return stego_image;
}

