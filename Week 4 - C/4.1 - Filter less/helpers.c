#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Prototypes
RGBTRIPLE to_grayscale(RGBTRIPLE pixel);
RGBTRIPLE to_sepia(RGBTRIPLE pixel);

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // For each image row
    for (int i = 0; i < height; i++)
    {
        // For each image column (aka pixel)
        for (int j = 0; j < width; j++)
        {
            image[i][j] = to_sepia(image[i][j]);
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

RGBTRIPLE to_sepia(RGBTRIPLE pixel)
{
    // Red
    int sepiaRed =
        (int) round(.393 * pixel.rgbtRed + .769 * pixel.rgbtGreen + .189 * pixel.rgbtBlue);
    if (sepiaRed > 255)
    {
        sepiaRed = 255;
    }

    // Green
    int sepiaGreen =
        (int) round(.349 * pixel.rgbtRed + .686 * pixel.rgbtGreen + .168 * pixel.rgbtBlue);
    if (sepiaGreen > 255)
    {
        sepiaGreen = 255;
    }

    // Blue
    int sepiaBlue =
        (int) round(.272 * pixel.rgbtRed + .534 * pixel.rgbtGreen + .131 * pixel.rgbtBlue);
    if (sepiaBlue > 255)
    {
        sepiaBlue = 255;
    }

    // Update pixel colors
    pixel.rgbtRed = sepiaRed;
    pixel.rgbtGreen = sepiaGreen;
    pixel.rgbtBlue = sepiaBlue;

    // Return pixel
    return pixel;
}
