#include "SecretImage.h"
#include <fstream>  // For file handling

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    width = image.get_width();
    height = image.get_height();

    // Hafızada yer ayırma işlemi
    int upperSize = (width * (height + 1)) / 2;
    int lowerSize = (width * (height - 1)) / 2;
    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    // Upper and lower triangular oluşturma
    int upperIndex = 0, lowerIndex = 0;
    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            if (y >= x) {  // Upper triangular part (including diagonal)
                upper_triangular[upperIndex++] = image.get_pixel(x, y);
            } else {  // Lower triangular part
                lower_triangular[lowerIndex++] = image.get_pixel(x, y);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int* upper, int* lower) {
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}

// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int upperIndex = 0, lowerIndex = 0;
    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            if (y >= x) {
                image.set_pixel(x, y, upper_triangular[upperIndex++]);
            } else {
                image.set_pixel(x, y, lower_triangular[lowerIndex++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int upperIndex = 0, lowerIndex = 0;
    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            if (y >= x) {
                upper_triangular[upperIndex++] = image.get_pixel(x, y);
            } else {
                lower_triangular[lowerIndex++] = image.get_pixel(x, y);
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);


    file << width << " " << height << "\n";

    int upperSize = width * (height + 1) / 2;
    int lowerSize = width * (height - 1) / 2;

    for (int i = 0; i < upperSize; ++i) {
        file << upper_triangular[i] << " ";
    }
    file << "\n";

    for (int i = 0; i < lowerSize; ++i) {
        file << lower_triangular[i] << " ";
    }

    file.close();


}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);

    int w, h;
    file >> w >> h;

    int upperSize = w * (w + 1) / 2;
    int lowerSize = w * (w - 1) / 2;

    int* upper_triangular = new int[upperSize];
    int* lower_triangular = new int[lowerSize];

    for (int i = 0; i < upperSize; ++i) {
        file >> upper_triangular[i];
    }

    for (int i = 0; i < lowerSize; ++i) {
        file >> lower_triangular[i];
    }

    file.close();

    SecretImage secretImage(w, h, upper_triangular, lower_triangular);
    return secretImage;
}

// Returns a pointer to the upper triangular part of the secret image.
int* SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int* SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
