#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
int check_key(string key);
bool contains_each_letter_once(string key);
string format_key(string raw_key);
string encrypt_string(string key, string s);
char rotate_char(string key, char c);

// Const
const int KEY_LENGTH = 26;

// Main
int main(int argc, string argv[])
{
    // Expected: arguments count = 2 (program name + key)
    if (argc != 2)
    {
        printf("/!\\Usage: ./substitution key)\n");
        return 1;
    }

    // Check key
    string key = argv[1];
    if (check_key(key) > 0)
    {
        return 1;
    }

    // Encrypt
    key = format_key(key);
    string plaintext = get_string("plaintext:  ");
    string ciphertext = encrypt_string(key, plaintext);

    // Output
    printf("ciphertext: %s\n", ciphertext);
}

// Check key format
int check_key(string key)
{
    // Expected: length = 26
    if (strlen(key) != KEY_LENGTH)
    {
        printf("/!\\ Key should be %i characters long\n", KEY_LENGTH);
        return 1;
    }

    // Expected: contains only alpha chars
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("/!\\ Key should only contains alphabetic characters\n");
            return 1;
        }
    }

    // Expected : contains each letter exactly once
    if (!contains_each_letter_once(key))
    {
        printf("/!\\ Key should contain each letter exactly once\n");
        return 1;
    }
    return 0;
}

// Return true if key contains each letter exactly once
bool contains_each_letter_once(string key)
{
    // Init matching array
    int ALPHABET_LENGTH = 26;
    int matchs[ALPHABET_LENGTH];
    for (int i = 0; i < ALPHABET_LENGTH; i++)
    {
        matchs[i] = 0;
    }

    // Check no doubled letter
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        int index = toupper(key[i]) - 'A';
        if (matchs[index] == 1)
        {
            return false;
        }
        matchs[index] = 1;
    }

    // Check all letters are represented (checksum = 26)
    for (int i = 0; i < ALPHABET_LENGTH; i++)
    {
        if (matchs[i] == 0)
        {
            return false;
        }
    }
    return true;
}

// Format key
string format_key(string raw_key)
{
    string key = raw_key;
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        // Case-insensitive key
        char upper_c = toupper(key[i]);
        // Compute rotation_key
        key[i] = upper_c - ('A' + i);
    }
    return key;
}

// Return ciphertext
string encrypt_string(string key, string s)
{
    // Init output
    int length = strlen(s);
    string ciphertext = s;

    // Encrypt alphabetical chars
    for (int i = 0; i < length; i++)
    {
        if (isalpha(s[i]))
        {
            ciphertext[i] = rotate_char(key, s[i]);
        }
    }

    // Result
    return ciphertext;
}

// Return rotated char
char rotate_char(string key, char c)
{
    // Check case
    char c_max = 'z';
    if (c < 'a')
    {
        c_max = 'Z';
    }

    // Rotate
    int index = toupper(c) - 'A';
    int steps = key[index];
    int cipherchar = c + steps;

    // Wrap around from 'z' to 'a' || 'Z' to 'A'
    if (cipherchar > c_max)
    {
        cipherchar -= 26;
    }

    // Result
    return (char) cipherchar;
}
