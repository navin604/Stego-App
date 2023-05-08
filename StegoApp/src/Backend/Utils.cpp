#include "Utils.h"
#include "Image.h"


void Stego::embed(std::vector<char> bits, PWSTR cover_file, char path[])
{
    cv::Mat cover_image = Image::getImageObj(cover_file);
    //Embed in cover image
    int check = 0;
    for (int i = 0; i < cover_image.rows; i++)
    {

        for (int j = 0; j < cover_image.cols; j++)
        {
            if (check >= bits.size())
            {
                break;
            }
            cv::Vec3b Pixel = cover_image.at<cv::Vec3b>(i, j);
            int r = Pixel[2];
            int g = Pixel[1];
            int b = Pixel[0];
            std::string rb = std::bitset<8>(r).to_string();


            int val = r;
            int replace_bit = bits[check] - '0'; // Replace the least significant bit 
            int result = (val & ~1) | replace_bit; // Clear the least significant bit of n and replace it with replace_bit
            Pixel[2] = result;
            check++;

            if (check != bits.size())
            {
                int val = g;
                int replace_bit = bits[check] - '0';  // Replace the least significant bit 
                int result = (val & ~1) | replace_bit; // Clear the least significant bit of n and replace it with replace_bit
                Pixel[1] = result;
                check++;
            }

            if (check != bits.size())
            {
                int val = b;
                int replace_bit = bits[check] - '0';  // Replace the least significant bit 
                int result = (val & ~1) | replace_bit; // Clear the least significant bit of n and replace it with replace_bit
                Pixel[0] = result;
                check++;
            }

            cover_image.at<cv::Vec3b>(i, j) = Pixel;


            // do something with BGR values...
        }
        if (check == bits.size())
        {
            std::cout << "\nExiting at " << check;
            break;
        }
    }
    //save new image
    cv::imwrite(path, cover_image);
}