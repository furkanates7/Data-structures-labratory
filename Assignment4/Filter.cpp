#define _USE_MATH_DEFINES

#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    int width = image.get_width();
    int height = image.get_height();
    std::vector<int> newData(width * height);
    int offset = kernelSize / 2;

    // Kernel uygulaması
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;
            int count = 0;

            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        sum += image.get_pixel(ny, nx);
                        count++;
                    } else {
                        sum += 0;
                        count++;
                    }
                }
            }

            newData[y * width + x] = sum / count;
        }
    }

    // Resmi güncelle
    for (int i = 0; i < width * height; ++i) {
        image.set_pixel(i / width, i % width, newData[i]);
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    int width = image.get_width();
    int height = image.get_height();
    std::vector<int> newData(width * height);
    int offset = kernelSize / 2;

    // Gaussian kernel uygulaması
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;

    for (int y = -offset; y <= offset; ++y) {
        for (int x = -offset; x <= offset; ++x) {
            double value;
            value = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            kernel[y + offset][x + offset] = value;
            sum += value;
        }
    }

    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double newValue = 0.0;
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        newValue += image.get_pixel(nx, ny) * kernel[ky + offset][kx + offset];
                    }
                }
            }
            newData[y * width + x] = static_cast<int>(newValue);
        }
    }

    // Resmi güncelle
    for (int i = 0; i < width * height; ++i) {
        image.set_pixel(i % width, i / width, newData[i]);
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    int width = image.get_width();
    int height = image.get_height();

    std::vector<int> newData(width * height);

    // Gaussian filtresi uygulaması
    GrayscaleImage blurredImage = image;
    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    // Unsharp mask uygulaması
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int originalPixel = image.get_pixel(x, y);
            int blurredPixel = blurredImage.get_pixel(x, y);


            int sharpenedPixel = originalPixel + (amount * ( originalPixel -blurredPixel ));
            sharpenedPixel = std::max(0, std::min(255, sharpenedPixel));


            image.set_pixel(x, y, sharpenedPixel);
        }
    }
}
