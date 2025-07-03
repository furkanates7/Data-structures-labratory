#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    GrayscaleImage image = secret_image.reconstruct();
    int width = image.get_width();
    int height = image.get_height();
    int total_pixels = width * height;
    int total_bits = message_length * 7;
    if (total_pixels < total_bits) {
        throw std::runtime_error("Not enough pixels in the image to store the message.");
    }
    int bit_index = 0;
    for (int x = height - 1; x >= 0 && bit_index < total_bits; --x) {
        for (int y = width - 1; y >= 0 && bit_index < total_bits; --y) {
            int pixel = image.get_pixel(x, y);
            int LSB = pixel & 1;  // Extract the LSB
            LSB_array.insert(LSB_array.begin(),LSB);
            ++bit_index;
        }
    }

    return LSB_array;
}





// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("Invalid LSB array size, must be multiple of 7.");
    }


    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int ascii_value = 0;
        for (int j = 0; j < 7; ++j) {
            ascii_value = (ascii_value << 1) | LSB_array[i + j];
        }
        message.push_back(static_cast<char>(ascii_value));
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    for (char c : message) {
        std::bitset<7> bits(c);
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(bits[i]);
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    int width = image.get_width();
    int height = image.get_height();

    if (LSB_array.size() > width * height) {
        throw std::runtime_error("Message is too long to embed in this image.");
    }


    int bit_index = LSB_array.size() - 1;

    for (int x = height - 1; x >= 0 && bit_index >= 0; --x) {
        for (int y = width - 1; y >= 0 && bit_index >= 0; --y) {
            int pixel = image.get_pixel(x, y);
            pixel = (pixel & 0xFE) | LSB_array[bit_index];
            image.set_pixel(x, y, pixel);
            --bit_index;
        }
    }

    SecretImage secretImage(image);
    return secretImage;
}
