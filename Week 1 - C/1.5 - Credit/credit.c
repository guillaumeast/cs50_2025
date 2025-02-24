#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number = get_long("Number: ");

    // Split number into digits (reversed order!)
    int digits[16];
    int digits_count = 0;
    while (number > 0)
    {
        digits[digits_count] = number % 10;
        number /= 10;
        digits_count++;
    }

    // Check Luhn's Algorithm
    int checksum = 0;

    // 1. Multiply half of the digits by 2 and sum those products' digits together
    // Starting with the number’s second-to-last digit
    for (int i = 1; i < digits_count; i += 2)
    {
        // Multiply current digit by 2
        int doubled_number = digits[i] * 2;

        // If results is 2 digits long => sum those digits
        if (doubled_number > 9)
        {
            doubled_number = 1 + (doubled_number % 10);
        }

        // Add the final result to checksum
        checksum += doubled_number;
    }

    // 2. Add the sum of the digits that weren’t multiplied by 2
    for (int j = 0; j < digits_count; j += 2)
    {
        checksum += digits[j];
    }

    // 3. Check last digit (should be 0)
    if (checksum % 10 == 0)
    {
        int first_two_digits = digits[digits_count - 1] * 10 + digits[digits_count - 2];
        if (digits[digits_count - 1] == 4 && (digits_count == 13 || digits_count == 16))
        {
            printf("VISA\n");
        }
        else if ((first_two_digits == 34 || first_two_digits == 37) && digits_count == 15)
        {
            printf("AMEX\n");
        }
        else if (first_two_digits > 50 && first_two_digits < 56 && digits_count == 16)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
