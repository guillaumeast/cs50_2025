#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Ask number of rows to print
    int rows;
    do
    {
        rows = get_int("Height: ");
    }
    while (rows < 1);

    // Print each row
    for (int i = 0; i < rows; i++)
    {
        // LEFT - Print each " "
        for (int j = 0; j < rows - 1 - i; j++)
        {
            printf(" ");
        }

        // LEFT - Print each "#"
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        // MIDDLE - Print "  "
        printf("  ");

        // RIGHT - Print each "#"
        for (int l = 0; l < i + 1; l++)
        {
            printf("#");
        }

        // Insert new line for next row
        printf("\n");
    }
}
