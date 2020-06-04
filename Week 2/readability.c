#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define STORAGE 10000

int main(void)
{
    char *input = malloc(STORAGE);
    if (input == NULL)
    {
        return 1;
    }

    printf("Text: ");
    fgets(input, STORAGE, stdin);

    int letters = 0, words = 0, sentences = 0, index = 0;
    float lOverW = 0, sOverW = 0;

    for (int i = 0; i < STORAGE; i++)
    {
        if (isalpha(input[i]))
        {
            letters++;
        }

        if (isspace(input[i]) != 0)
        {
            words++;
        }

        if (input[i] == '.' || input[i] == '?' || input[i] == '!')
        {
            sentences++;
        }
    }

    lOverW = (float)letters / (float)words * 100;
    sOverW = (float)sentences / (float)words * 100;
    index = (int)(round(0.0588 * lOverW - 0.296 * sOverW - 15.8));

    // printf("%i letter(s)\n", letters);
    // printf("%i word(s)\n", words);
    // printf("%i sentence(s)\n", sentences);

    free(input);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}
