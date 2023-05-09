#include "Utils.h"
#include "Image.h"
#include "Validation.h"


void Stego::embed(std::vector<char> bits, PWSTR cover_file, char path[])
{
    // Retrieve cover image
    cv::Mat cover_image = Image::getImageObj(cover_file);
   
    // Iterates through R, G, and B values in each pixel of cover image, 
    // embedding bits from bits vector in the place of Least Significant Bit
    int check = 0;
    for (int i = 0; i < cover_image.rows; i++)
    {

        for (int j = 0; j < cover_image.cols; j++)
        {
            if (check >= bits.size())
            {
                break;
            }
            // Get pixel
            cv::Vec3b Pixel = cover_image.at<cv::Vec3b>(i, j);

            // int values of RGB
            int r = Pixel[2];
            int g = Pixel[1];
            int b = Pixel[0];

            // Embed bits
            std::string rb = std::bitset<8>(r).to_string();
            int val = r;
            int replace_bit = bits[check] - '0'; 
            int result = (val & ~1) | replace_bit; 
            Pixel[2] = result;
            check++;

            if (check != bits.size())
            {
                int val = g;
                int replace_bit = bits[check] - '0'; 
                int result = (val & ~1) | replace_bit; 
                Pixel[1] = result;
                check++;
            }

            if (check != bits.size())
            {
                int val = b;
                int replace_bit = bits[check] - '0';  
                int result = (val & ~1) | replace_bit; 
                Pixel[0] = result;
                check++;
            }

            // Overwrite current pixel with the pixel
            // containing embedded data
            cover_image.at<cv::Vec3b>(i, j) = Pixel;


            
        }
        // Check if all bits have been embedded
        if (check == bits.size())
        {
            std::cout << "\nExiting at " << check;
            break;
        }
    }
    // Save image to file
    Image::save_image(path, cover_image);
    
}

void Stego::decode(char path[])
{
    // Retrieve image to decode
    cv::Mat stego_image = Image::open_image(path);
    
    // Contains bit strings read from stego_image
    std::map<std::string, std::string> message_strings = {
       {"message_size",""},
       {"width", ""},
       {"height", ""},
       {"file_name_size", ""},
       {"file_name", ""},
       {"image", ""},
    };

    // Specifies at which bit a specific message has been fully
    // received. file_name and image will be determined upon
    // processing message_size and file_name_size.
    std::map<std::string, int> message_types = {
        {"message_size", 33},
        {"width", 46},
        {"height", 59},
        {"file_name_size", 91},
        {"file_name", 0},
        {"image", 0}


    };

    // Specifies which value in message_strings is being processed
    std::string current_type = "message_size";

    // When true, decoding finished
    bool complete = false;


    int current_bit = 1;

    // Retrieve data
    for (int i = 0; i < stego_image.rows; i++)
    {
        for (int j = 0; j < stego_image.cols; j++)
        {
            if (complete)
            {
                goto end_loop;
            }

            // Get pixel
            cv::Vec3b Pixel = stego_image.at<cv::Vec3b>(i, j);

            // Extract least significant bit and process
            int r_lsb = Pixel[2] & 1;
            process_bit(current_bit, r_lsb, current_type, message_strings, complete, message_types);
            int g_lsb = Pixel[1] & 1;
            process_bit(current_bit, g_lsb, current_type, message_strings, complete, message_types);
            int b_lsb = Pixel[0] & 1;
            process_bit(current_bit, b_lsb, current_type, message_strings, complete, message_types);


        }
    }
    end_loop:;

    // Removes last bit from bit vector as it is not needed
    message_strings["image"].pop_back();
    std::cout << message_strings["image"].length() << "awdawdwad\n";
    // Get width and height of embedded image
    int width = Validation::bin_to_int(message_strings["width"]);
    int height = Validation::bin_to_int(message_strings["height"]);





    // Create empty black image using data extracted from stego image
    cv::Mat decoded_image = Image::create_image(height, width);

  
    // Fill in empty image using bits from image bit string
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Calculate index of pixel (Taken from opencv documentation)
            int index = (row * width + col) * 8 * 3;

            // Extract r, g, and b values
            int r = std::bitset<8>(message_strings["image"].substr(index, 8)).to_ulong();
            int g = std::bitset<8>(message_strings["image"].substr(index + 8, 8)).to_ulong();
            int b = std::bitset<8>(message_strings["image"].substr(index + 16, 8)).to_ulong();

            // Set pixel value 
            decoded_image.at<cv::Vec3b>(row, col) = cv::Vec3b(b, g, r);
        }
    }
    cv::imwrite(message_strings["file_name"], decoded_image);

}


void Stego::set_file_name(std::map<std::string, std::string>& message_strings)
{
    // Extract file name from bits. Every 8
    // bits represents a character from the filename
    std::string cur = "";
    std::string filename = "";

    for (int i = 0; i <= message_strings["file_name"].size(); i++)
    {
        if (i != 0 && i % 8 == 0)
        {
            // Convert 8 bits into char
            int ASCII_code = std::stoi(cur, nullptr, 2);
            char ASCII_SYM = static_cast<char>(ASCII_code);
            filename += ASCII_code;
            cur.clear();
        }
        cur += message_strings["file_name"][i];
    }


    message_strings["file_name"] = filename;
}

void Stego::change_type(std::string& current_type, std::map<std::string, std::string>& message_strings, std::map<std::string, int>& message_types, bool& complete) {
    if (current_type == "message_size")
    {
        // Finished extracting message_size, set next task
        int size;
        message_types["image"] = std::bitset<32>(message_strings[current_type]).to_ulong();
        current_type = "width";
        std::cout << "msg size: " << message_types["image"] << "\n";

    }
    else if (current_type == "width")
    {
        // Finished extracting width of image, set next task
        int size;
        size = std::bitset<32>(message_strings[current_type]).to_ulong();
        std::cout << "width size: " << size << "\n";
        current_type = "height";
    }
    else if (current_type == "height")
    {
        // Finished extracting height of image, set next task
        int size;
        size = std::bitset<32>(message_strings[current_type]).to_ulong();
        std::cout << "height size: " << size << "\n";
        current_type = "file_name_size";
    }
    else if (current_type == "file_name_size")
    {
        // Finished size of file name (length)
        int size;
        size = std::bitset<32>(message_strings[current_type]).to_ulong();
        std::cout << "file name size: " << size << "\n";

        current_type = "file_name";
        message_types[current_type] = size + message_types["file_name_size"];
        message_types["image"] = message_types[current_type] + message_types["image"];
    }
    else if (current_type == "file_name")
    {
        // Finished extracting filename
        set_file_name(message_strings);
        current_type = "image";
    }
    else {
        // Finished extracting image
        complete = true;
    }
}



void Stego::process_bit(int& current_bit, int lsb, std::string& current_type, std::map<std::string, std::string>& message_strings, bool& complete, std::map<std::string, int>& message_types)
{
    // All data has been retrieved
    if (complete)
    {
        return;
    }
    else if (current_bit == message_types[current_type])
    {
        // All data for current message has been retrieved
        change_type(current_type, message_strings, message_types, complete);

    }
    // Add bit to relevant 
    message_strings[current_type] += std::to_string(lsb);
    current_bit++;


}