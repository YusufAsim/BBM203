#include "GrayscaleImage.h"
#include <iostream>
#include <cstring> // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>

// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char *filename)
{

    // Image loading code using stbi
    int channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr)
    {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // TODO: Your code goes here.
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    // Fill the matrix with pixel values from the image

    this->data = new int *[height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new int[width];
        for (int j = 0; j < width; j++)
        {
            data[i][j] = image[(i * height) + j];
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int **inputData, int h, int w)
{
    // TODO: Your code goes here.
    // Initialize the image with a pre-existing data matrix by copying the values.
    // Don't forget to dynamically allocate memory for the matrix.

    this->data = new int *[h];
    for (int i = 0; i < h; i++)
    {
        this->data[i] = new int[w];
        for (int j = 0; j < w; j++)
        {
            this->data[i][j] = inputData[i][j];
        }
    }

    this->height = h;
    this->width = w;
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h)
{
    // TODO: Your code goes here.
    // Just dynamically allocate the memory for the new matrix.
    this->data = new int *[h];
    for (int i = 0; i < h; i++)
    {
        data[i] = new int[w];
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage &other)
{
    // TODO: Your code goes here.
    // Copy constructor: dynamically allocate memory and
    // copy pixel values from another image.
    this->data = new int *[other.height];
    for (int i = 0; i < other.height; ++i)
    {
        this->data[i] = new int[other.width];
        // Copy the data from the other image
        for (int j = 0; j < other.width; ++j)
        {
            this->set_pixel(i, j, other.get_pixel(i, j));
        }
    }

    this->height = other.height;
    this->width = other.width;
}

// Destructor
GrayscaleImage::~GrayscaleImage()
{
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.

    for (int i = 0; i < this->height; i++)
    {
        delete[] this->data[i];
    }
    delete[] this->data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage &other) const
{
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.

    if (!(this->height == other.height) && (this->width == other.width))
    {
        return false;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (this->data[i][j] != other.data[i][j])

            {
                return false;
            }
        }
    }

    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage &other) const
{
    // Create a new image for the result
    GrayscaleImage result(width, height);

    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum = this->data[i][j] + other.data[i][j];
            if (sum > 255)
            {
                result.data[i][j] = 255;
            }
            else
            {
                result.data[i][j] = sum;
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage &other) const
{
    // Create a new image for the result
    GrayscaleImage result(width, height);

    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int substraction = this->data[i][j] - other.data[i][j];
            if (substraction < 0)
            {
                result.data[i][j] = 0;
            }
            else
            {
                result.data[i][j] = substraction;
            }
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const
{
    return this->data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value)
{
    this->data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char *filename) const
{
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char *imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width))
    {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
