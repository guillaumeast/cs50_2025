#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Prototypes
int get_word_score(string s);
int get_char_score(char c);

// Data
int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Main
int main(void)
{
    // Ask player words
    string word_1 = get_string("Player 1: ");
    string word_2 = get_string("Player 2: ");

    // Compute scores
    int score_1 = get_word_score(word_1);
    int score_2 = get_word_score(word_2);

    // Output result
    if (score_1 > score_2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score_1 < score_2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// Return total score of a word
int get_word_score(string s)
{
    int score = 0;
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        score += get_char_score(s[i]);
    }
    return score;
}

// Return score of a letter
int get_char_score(char c)
{
    if (isalpha(c))
    {
        char capitalised = toupper(c);
        int ascii_hex = (int) capitalised;
        // Ascii_hex of 'A' = 65
        return points[ascii_hex - 65];
    }
    else
    {
        return 0;
    }
}
