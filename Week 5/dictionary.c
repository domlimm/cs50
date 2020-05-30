// Implements a dictionary's functionality

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26; // all the alphabets A-Z

// Hash table
node *table[N]; // Array of node pointers

int numWordsDict = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int destination = hash(word);

    node *tempNode = table[destination];

    if (strcasecmp(tempNode->word, word) == 0)
    {
        return true;
    }

    while (tempNode->next != NULL)
    {
        tempNode = tempNode->next;

        if (strcasecmp(tempNode->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return (int) (toupper(word[0]) - 65);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char *tempWord = malloc(LENGTH);
    if (tempWord == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", tempWord) != EOF)
    {
        node *tempNode = malloc(sizeof(node));
        if (tempNode == NULL)
        {
            return false;
        }

        strcpy(tempNode->word, tempWord); // set word
        tempNode->next = table[hash(tempWord)]; // then link to next point in the list

        table[hash(tempWord)] = tempNode;

        numWordsDict++;
    }

    free(tempWord);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return numWordsDict;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *cursorNode;
    node *tempNode;

    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        cursorNode = table[i];
        tempNode = cursorNode;

        while (cursorNode->next != NULL)
        {
            cursorNode = cursorNode->next;
            free(tempNode);
            tempNode = cursorNode;
        }

        free(cursorNode);
    }

    return true;
}
