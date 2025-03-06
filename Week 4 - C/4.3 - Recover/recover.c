#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Types
typedef uint8_t BYTE;

// Const
const int BLOCK_SIZE = 512;

// Prototypes
int is_jpg(BYTE bytes[BLOCK_SIZE]);
int copy_images(BYTE buffer[BLOCK_SIZE], FILE *card);
FILE *create_jpg_file(int index);

// Main
int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: ./recover forensic_image\n");
        return 1;
    }

    // Open forensic image
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Error: Can't open forensic image\n");
        return 1;
    }

    // Search for first jpeg
    BYTE buffer[BLOCK_SIZE];
    int items_read;
    do
    {
        items_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, card);
    }
    while (items_read == BLOCK_SIZE && is_jpg(buffer) == 0);

    // Copy images
    copy_images(buffer, card);

    // Close forensic image
    fclose(card);
}

// Returns 1 if jpeg signature is found (0 otherwise)
int is_jpg(BYTE bytes[BLOCK_SIZE])
{
    // Check first three bytes
    if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
    {
        // Check fourth byte's first four bits
        if ((bytes[3] & 0xf0) == 0xe0)
        {
            return 1;
        }
    }

    // No jpg signature found
    return 0;
}

// Copy images until end of memory card
int copy_images(BYTE buffer[BLOCK_SIZE], FILE *card)
{
    int index = 0;
    int bytes_read;
    do
    {
        // Create image
        FILE *image = create_jpg_file(index);

        // Copy data
        do
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, image);
            bytes_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, card);
        }
        while (bytes_read > 0 && is_jpg(buffer) == 0);

        // Close jpg file and increment index
        fclose(image);
        index++;
    }
    while (bytes_read == BLOCK_SIZE);

    return 0;
}

// Returns pointer to new jpg file with formatted name
FILE *create_jpg_file(int index)
{
    // Compute file name
    char name[8];
    sprintf(name, "%03i.jpg", index);

    // Create jpg file
    FILE *image = fopen(name, "w");
    if (image == NULL)
    {
        printf("Error: can't create new jpg file");
        exit(1);
    }

    // Return pointer to created file
    return image;
}
