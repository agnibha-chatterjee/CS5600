/**
 * polybius.c / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "polybius.h"

char *pbEncode(const char *plaintext, const polybius_table_t *table)
{
    if (plaintext == NULL || table == NULL) {
        return NULL;
    }

    int len = (int)strlen(plaintext);
   
    char *encoded = malloc(3 * len + 1);
    if (!encoded)
        return NULL;

    int pos = 0;
    for (int i = 0; i < len; i++)
    {
        char ch = plaintext[i];
        if (isalpha(ch))
        {
            ch = toupper(ch);
            if (ch == 'J') {
                ch = 'I';
            }
            int found = 0;
            for (int row = 0; row < POLYBIUS_SIZE && !found; row++)
            {
                for (int col = 0; col < POLYBIUS_SIZE && !found; col++)
                {
                    if (table->square[row][col] == ch)
                    {
                        encoded[pos++] = '1' + row;
                        encoded[pos++] = '1' + col;
                        found = 1;
                    }
                }
            }
            if (!found)
            {
                encoded[pos++] = ch;
            }
        }
        else
        {
            encoded[pos++] = ch;
        }
    }
    encoded[pos] = '\0';
    return encoded;
}

char *pbDecode(const char *ciphertext, const polybius_table_t *table)
{
    if (ciphertext == NULL || table == NULL) {
        return NULL;
    }

    int len = (int)strlen(ciphertext);
    char *decoded = malloc(len + 1);
    if (!decoded) {
        return NULL;
    }

    int pos = 0;
    for (int i = 0; i < len; i++)
    {
        if (isdigit(ciphertext[i]))
        {
            if ((i + 1) < len && isdigit(ciphertext[i + 1]))
            {
                int row = ciphertext[i] - '1';
                int col = ciphertext[i + 1] - '1';
                i++;
                if (row >= 0 && row < POLYBIUS_SIZE && col >= 0 && col < POLYBIUS_SIZE)
                {
                    char letter = table->square[row][col];
                    decoded[pos++] = letter;
                }
                else
                {
                    decoded[pos++] = '?';
                }
            }
            else
            {
                decoded[pos++] = ciphertext[i];
            }
        }
        else
        {
            decoded[pos++] = ciphertext[i];
        }
    }
    decoded[pos] = '\0';
    return decoded;
}