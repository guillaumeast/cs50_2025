// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 14348907;

// Hash table
node *table[N];

// Hard coded pows of 27 to optimise hash function (26 alpha chars + apostrophe)
int POWS[] = {1, 27, 729, 19683, 531441};

// Loaded dictionary words
int DICTIONARY_WORD_COUNT = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }

    // Save each word in the hashmap
    char buffer[LENGTH + 1];
    while (fscanf(f, "%s", buffer) != EOF)
    {
        if (insert(buffer) == false)
        {
            fclose(f);
            return false;
        }
    }

    // Close file
    fclose(f);
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;

    for (int i = 0; i < 5; i++)
    {
        // End of word
        if (word[i] == '\0')
        {
            break;
        }

        // Hash ith char
        if (word[i] == '\'')
        {
            sum += 26 * POWS[i];
        }
        else
        {
            sum += (toupper(word[i]) - 'A') * POWS[i];
        }
    }

    return sum;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Lowercase
    int length = strlen(word);
    char lowered[length + 1];
    for (int i = 0; i < length; i++)
    {
        lowered[i] = tolower(word[i]);
    }
    lowered[length] = '\0';

    // Comparing each word of linked list
    node *ptr = table[hash(lowered)];
    while (ptr != NULL)
    {
        if (strcmp(ptr->word, lowered) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return DICTIONARY_WORD_COUNT;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // For each bucket
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        // Free each linked list element
        while (ptr != NULL)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }
    return true;
}

//////////////////////////////////
///      CUSTOM FUNCTIONS      ///
//////////////////////////////////

// Returns true if the word has been added to the linked list in hashmap
bool insert(const char *word)
{
    // Create node
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        return false;
    }

    // Update node
    int i = hash(word);
    strcpy(n->word, word);
    n->next = table[i];

    // Insert node
    table[i] = n;

    // Increment loaded dictionary word count
    DICTIONARY_WORD_COUNT++;
    return true;
}
