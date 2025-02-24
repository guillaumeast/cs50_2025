#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
bool is_decimal(string s);
string encrypt_string(int key, string s);
char rotate(int key, char c);

// Main
int main(int argc, string argv[])
{
    // Check argc (expected: 2)
    if (argc != 2)
    {
        printf("Error, argc should be 2");
        return 1;
    }

    // Check key (expected: only digits chars)
    if (!is_decimal(argv[1]))
    {
        printf("Usage: ./caesar key");
        return 1;
    }

    // Encrypt
    string plaintext = get_string("plaintext:  ");
    int key = atoi(argv[1]);
    string ciphertext = encrypt_string(key, plaintext);

    // Output
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

// Return true if s only contains decimal digits
bool is_decimal(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Return ciphertext
string encrypt_string(int key, string s)
{
    // Wrap key
    int k = key % 26;

    // Init output
    int length = strlen(s);
    string encrypted = s;

    // Encrypt alphabetical chars
    for (int i = 0; i < length; i++)
    {
        if (isalpha(s[i]))
        {
            encrypted[i] = rotate(k, s[i]);
        }
    }

    // Result
    return encrypted;
}

// Return rotated char
char rotate(int key, char c)
{
    // Check case
    char c_max = 'z';
    if (c < 'a')
    {
        c_max = 'Z';
    }

    // Rotate
    int encrypted = c + key;

    // Wrap around from 'z' to 'a' || 'Z' to 'A'
    if (encrypted > c_max)
    {
        encrypted -= 26;
    }

    // Result
    return (char) encrypted;
}
