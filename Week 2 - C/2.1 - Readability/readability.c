#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Prototypes
int get_words_count(int length, string s);
int get_char_count(int length, string s);
int get_sentences_count(int length, string s);
int get_grade(string s);
void output_result(int grade);

// Main
int main(void)
{
    // Ask for text
    string text = get_string("Text: ");

    // Compute grade
    int grade = get_grade(text);

    // Output
    output_result(grade);
}

// Return grade based on the Coleman-Liau index
int get_grade(string s)
{
    int length = strlen(s);

    // Compute words_count
    int words_count = get_words_count(length, s);

    // 1. Compute L (average number of letters per 100 words)
    int letters_count = get_char_count(length, s);
    float L = (float) letters_count / words_count * 100;

    // 2. Compute S (average number of sentences per 100 words)
    int sentences_count = get_sentences_count(length, s);
    float S = (float) sentences_count / words_count * 100;

    // Compute Compute Coleman-Liau index
    int grade = round(0.0588 * L - 0.296 * S - 15.8);
    printf("===> L = %f\n", L);
    printf("===> S = %f\n", S);
    return grade;
}

// Return the number of words of a text
int get_words_count(int length, string s)
{
    int words_count = 1;
    for (int i = 0; i < length; i++)
    {
        if (isblank(s[i]))
        {
            words_count++;
        }
    }
    printf("-> Words = %i\n", words_count);
    return words_count;
}

// Return the number of letter of a word
int get_char_count(int length, string s)
{
    int char_count = 0;
    for (int i = 0; i < length; i++)
    {
        if (isalpha(s[i]))
        {
            char_count++;
        }
    }
    printf("-> Letters = %i\n", char_count);
    return char_count;
}

// Return the number of sentences of a text
int get_sentences_count(int length, string s)
{
    int sentences_count = 0;
    for (int i = 0; i < length; i++)
    {
        char c = s[i];
        if (c == '.' || c == '?' || c == '!')
        {
            sentences_count++;
        }
    }
    printf("-> Sentences = %i\n", sentences_count);
    return sentences_count;
}

// Output result
void output_result(int grade)
{
    printf("--------------------");
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
