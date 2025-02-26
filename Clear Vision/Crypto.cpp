#include "Crypto.h"
#include "GrayscaleImage.h"

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.
    GrayscaleImage image = secret_image.reconstruct();

    int height = image.get_height(), width = image.get_width();
    int number_of_bits = 7 * message_length;
    int start_index_row = height - 1 - (number_of_bits / height);
    int start_index_column = width - (number_of_bits - (height - (start_index_row + 1)) * height);

    if (number_of_bits > height * width)
    {
        throw std::runtime_error("Image hasn't got enough pixels.");
    }

    for (int i = start_index_row; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            if (i == start_index_row && j < start_index_column)
            {
                continue;
            }

            int pixel_value = image.get_pixel(i, j);

            int lsb = pixel_value & 1; // get the LSB
            LSB_array.push_back(lsb);
        }
    }
    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int> &LSB_array)
{

    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message
    if (LSB_array.size() % 7 != 0)
    {
        throw std::runtime_error("Array size is not a multiple of 7.");
    }

    for (int i = 0; i < LSB_array.size(); i += 7)
    {
        std::bitset<7> bit_form;
        for (int j = 0; j < 7; j++)
        {
            bit_form[6 - j] = LSB_array[i + j];
        }
        char a_char = static_cast<char>(bit_form.to_ulong());
        message += a_char;
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string &message)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.

    for (char character : message)
    {
        std::bitset<7> bit_notation(character);
        for (int i = 0; i < 7; i++)
        {
            LSB_array.push_back(bit_notation[6 - i]);
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array)
{
    // TODO: Your code goes here.

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.

    if (image.get_height() * image.get_width() < LSB_array.size())
    {
        throw std::runtime_error("Image hasn't got enough pixels.");
    }

    int not_filled_pixel_number = image.get_height() * image.get_width() - LSB_array.size();
    int start_index_row = (not_filled_pixel_number + 1) / image.get_height();
    int start_index_column = not_filled_pixel_number % image.get_width();

    int idx = 0;

    for (int i = start_index_row; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            if (i == start_index_row && j < start_index_column)
            {
                continue;
            }

            int pixel_value = image.get_pixel(i, j);

            pixel_value = pixel_value & ~(1 << 0); // lsb = 0

            pixel_value = pixel_value | LSB_array.at(idx++); // lsb change

            image.set_pixel(i, j, pixel_value);
        }
    }
    SecretImage secret_image(image);
    return secret_image;
}
