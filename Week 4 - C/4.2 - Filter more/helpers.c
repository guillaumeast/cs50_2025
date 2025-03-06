#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Prototypes
RGBTRIPLE to_grayscale(RGBTRIPLE pixel);
BYTE compute_color(BYTE grid[3][3]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For each image row
    for (int i = 0; i < height; i++)
    {
        // For each image column (aka pixel)
        for (int j = 0; j < width; j++)
        {
            image[i][j] = to_grayscale(image[i][j]);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For each row
    for (int i = 0; i < height; i++)
    {
        // Create buffer
        RGBTRIPLE buffer[width];

        // Populate buffer (reverse pixels)
        for (int j = 0; j < width; j++)
        {
            buffer[j] = image[i][width - 1 - j];
        }

        // Update row
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // For each image row
    for (int i = 0; i < height; i++)
    {
        // For each image column (aka pixel)
        for (int j = 0; j < width; j++)
        {
            // Init
            int red = 0;
            int green = 0;
            int blue = 0;
            float count = 0;

            // Loop through previous, current and next row
            for (int h = i - 1; h <= i + 1; h++)
            {
                // If this row is within the image
                if (h >= 0 && h < height)
                {
                    // Loop through previous, current and next pixel
                    for (int w = j - 1; w <= j + 1; w++)
                    {
                        // If this pixel is within the image
                        if (w >= 0 && w < width)
                        {
                            red += image[h][w].rgbtRed;
                            green += image[h][w].rgbtGreen;
                            blue += image[h][w].rgbtBlue;
                            count++;
                        }
                    }
                }
            }

            // Update pixel colors value of copied image
            copy[i][j].rgbtRed = (BYTE) round(red / count);
            copy[i][j].rgbtGreen = (BYTE) round(green / count);
            copy[i][j].rgbtBlue = (BYTE) round(blue / count);
        }
    }

    // Update original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
}

RGBTRIPLE to_grayscale(RGBTRIPLE pixel)
{
    BYTE average = (int) round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
    pixel.rgbtBlue = average;
    pixel.rgbtGreen = average;
    pixel.rgbtRed = average;

    return pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Compute pixels
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Create a 3x3 grid for each color
            BYTE red_grid[3][3];
            BYTE green_grid[3][3];
            BYTE blue_grid[3][3];

            // Populate grids
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    // Pixel coordonates for this grid[i][j]
                    int h = row - 1 + i;
                    int w = col - 1 + j;

                    // Value for this grid[i][j]
                    if (h < 0 || h >= height || w < 0 || w >= width)
                    {
                        // Black if outside of image edges
                        red_grid[i][j] = 0;
                        green_grid[i][j] = 0;
                        blue_grid[i][j] = 0;
                    }
                    else
                    {
                        // Color values if inside of image edges
                        red_grid[i][j] = copy[h][w].rgbtRed;
                        green_grid[i][j] = copy[h][w].rgbtGreen;
                        blue_grid[i][j] = copy[h][w].rgbtBlue;
                    }
                }
            }

            // Compute with Sobel operator
            image[row][col].rgbtRed = compute_color(red_grid);
            image[row][col].rgbtGreen = compute_color(green_grid);
            image[row][col].rgbtBlue = compute_color(blue_grid);
        }
    }
}

// Sobel operator (one color at a time)
BYTE compute_color(BYTE grid[3][3])
{
    // Init
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Multiply and sum the resulting values
    int sum_x = 0;
    int sum_y = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sum_x += Gx[i][j] * grid[i][j];
            sum_y += Gy[i][j] * grid[i][j];
        }
    }

    // Result
    double square = sqrt(sum_x * sum_x + sum_y * sum_y);
    int result = round(square);
    if (result > 255)
    {
        result = 255;
    }

    // Return
    return (BYTE) result;
}
