#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

#include <iostream>

double calculate_weight(int kernel_size, double sigma, int i, int j)
{
    double x = abs(kernel_size / 2 - i), y = abs(kernel_size / 2 - j);
    double weight = exp(-1.0 * (pow(x, 2.0) + pow(y, 2.0)) / (2 * pow(sigma, 2.0))) / (2 * M_PI * pow(sigma, 2.0));
    return weight;
}

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage &image, int kernel_size)
{
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
    GrayscaleImage unfiltered_image = image;

    int number_of_row = unfiltered_image.get_height(), number_of_column = unfiltered_image.get_width();
    for (int i = 0; i < number_of_row; i++)
    {
        for (int j = 0; j < number_of_column; j++)
        {
            int sum = 0;
            int leftmost_pixel_i = i - (kernel_size - 1) / 2, leftmost_pixel_j = j - (kernel_size - 1) / 2;
            for (int x = leftmost_pixel_i; x < leftmost_pixel_i + kernel_size; x++)
            {
                for (int y = leftmost_pixel_j; y < leftmost_pixel_j + kernel_size; y++)
                {
                    if (x >= 0 && y >= 0 && x < number_of_row && y < number_of_column)
                    {
                        int number = unfiltered_image.get_pixel(x, y);
                        sum += number;
                    }
                }
            }
            image.set_pixel(i, j, sum / pow(kernel_size, 2));
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernel_size, double sigma)
{
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage unfiltered_image = image;
    double K_sum = 0;
    double **kernel = new double *[kernel_size];
    for (int i = 0; i < kernel_size; i++)
    {
        kernel[i] = new double[kernel_size];
        for (int j = 0; j < kernel_size; j++)
        {
            kernel[i][j] = calculate_weight(kernel_size, sigma, i, j);
            K_sum += kernel[i][j];
        }
    }

    int number_of_row = unfiltered_image.get_height(), number_of_column = unfiltered_image.get_width();
    for (int i = 0; i < number_of_row; i++)
    {
        int leftmost_pixel_i = i - kernel_size / 2;
        for (int j = 0; j < number_of_column; j++)
        {
            int leftmost_pixel_j = j - kernel_size / 2;
            double sum = 0;

            for (int x = leftmost_pixel_i; x < leftmost_pixel_i + kernel_size; x++)
            {
                for (int y = leftmost_pixel_j; y < leftmost_pixel_j + kernel_size; y++)
                {
                    if (x >= 0 && y >= 0 && x < number_of_row && y < number_of_column)
                    {
                        int number = unfiltered_image.get_pixel(x, y);
                        sum += number * kernel[x - leftmost_pixel_i][y - leftmost_pixel_j];
                    }
                }
            }
            image.set_pixel(i, j, sum / K_sum);
        }
    }

    for (int i = 0; i < kernel_size; i++)
    {
        delete[] kernel[i];
    }
    delete[] kernel;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernel_size, double amount)
{
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].

    GrayscaleImage gaussianImage(image); // creating the copy of image

    Filter::apply_gaussian_smoothing(gaussianImage, kernel_size); // make gauss filter

    GrayscaleImage edgeImage = image - gaussianImage; // (original - blurred)

    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            int original_form = image.get_pixel(i, j);
            int gaussian_filtered = gaussianImage.get_pixel(i, j);
            int formulated_target = original_form + amount * (original_form - gaussian_filtered);

            if (formulated_target >= 0 && formulated_target <= 255)
            {
                image.set_pixel(i, j, formulated_target);
            }
            else if (formulated_target < 0)
            {
                image.set_pixel(i, j, 0);
            }
            else
            {
                image.set_pixel(i, j, 255);
            }
        }
    }
}
