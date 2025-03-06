// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// Prototypes
void copy_header(FILE *input, FILE *output);
void copy_samples(FILE *input, FILE *output, float factor);

// Main
int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    copy_header(input, output);

    // Read samples from input file and write updated data to output file
    copy_samples(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

void copy_header(FILE *input, FILE *output)
{
    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);
}

void copy_samples(FILE *input, FILE *output, float factor)
{
    // Create a buffer for a signle sample
    int16_t buffer;

    // Read single sample from input into buffer while there are samples left to read
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // Update volume
        buffer *= factor;

        // Write updated sample to the new file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }
}
