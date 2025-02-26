#include "SecretImage.h"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage &image)
{
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    this->height = image.get_height();
    this->width = image.get_width();

    this->upper_triangular = new int[width * (width + 1) / 2];
    this->lower_triangular = new int[width * (width - 1) / 2]; // by excluding the diagonal of the matrice

    int index_u = 0;
    int index_l = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i <= j) // Upper triangular part
            {
                this->upper_triangular[index_u++] = image.get_pixel(i, j);
            }
            else // Lower triangular part
            {
                this->lower_triangular[index_l++] = image.get_pixel(i, j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int *upper, int *lower)
{
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.

    this->height = h;
    this->width = w;
    this->upper_triangular = upper;
    this->lower_triangular = lower;
}

// Destructor: free the arrays
SecretImage::~SecretImage()
{ // TODO: Your code goes here.
  // Simply free the dynamically allocated memory
  // for the upper and lower triangular matrices.

    delete[] this->upper_triangular;
    delete[] this->lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const
{
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.

    int indexu = 0, indexl = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i <= j) // Upper triangular part
            {
                image.set_pixel(i, j, upper_triangular[indexu++]);
            }
            else // Lower triangular part
            {
                image.set_pixel(i, j, lower_triangular[indexl++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage &image)
{
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.
    int indexu = 0, indexl = 0;
    for (int i = 0; i < image.get_height(); i++)
    {
        for (int j = 0; j < image.get_width(); j++)
        {
            if (i <= j) // Upper triangular part
            {
                upper_triangular[indexu++] = image.get_pixel(i, j);
            }
            else // Lower triangular part
            {
                lower_triangular[indexl++] = image.get_pixel(i, j);
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string &filename)
{
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a single space.
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    std::ofstream outputFile(filename);

    outputFile << width << " " << height << std::endl;

    int upper_size = (width * (width + 1)) / 2;
    for (int i = 0; i < upper_size; ++i)
    {
        outputFile << upper_triangular[i];
        if (i < upper_size - 1)
        {
            outputFile << " ";
        }
    }
    outputFile << std::endl;

    int lower_size = (height * (height - 1)) / 2;
    for (int i = 0; i < lower_size; ++i)
    {
        outputFile << lower_triangular[i];
        if (i < lower_size - 1)
        {
            outputFile << " ";
        }
    }

    outputFile.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string &filename)
{
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    std::ifstream current_file(filename);
    int width, height;
    current_file >> width >> height;

    int size_of_upper = width * (width + 1) / 2, size_of_lower = width * (width - 1) / 2;

    int *upper_triangular_matrice = new int[size_of_upper];
    int *lower_triangular_matrice = new int[size_of_lower];

    for (int i = 0; i < size_of_upper; ++i)
    {
        current_file >> upper_triangular_matrice[i];
    }

    for (int i = 0; i < size_of_lower; ++i)
    {
        current_file >> lower_triangular_matrice[i];
    }

    current_file.close();

    SecretImage secret_image(width, height, upper_triangular_matrice, lower_triangular_matrice);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int *SecretImage::get_upper_triangular() const
{
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int *SecretImage::get_lower_triangular() const
{
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const
{
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const
{
    return height;
}
