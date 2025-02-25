/**
 * cipher.c / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        char *option = argv[1];
        char *input = argv[2];

        // Hard-coded Polybius square
        polybius_table_t table = {
            .square = {
                {'A', 'B', 'C', 'D', 'E'},
                {'F', 'G', 'H', 'I', 'K'},
                {'L', 'M', 'N', 'O', 'P'},
                {'Q', 'R', 'S', 'T', 'U'},
                {'V', 'W', 'X', 'Y', 'Z'}
            }
        };

        char *result = NULL;
        if (strcmp(option, "-e") == 0)
        {
            result = pbEncode(input, &table);
        }
        else if (strcmp(option, "-d") == 0)
        {
            result = pbDecode(input, &table);
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\nUsage: %s -e|-d \"text\"\n", option, argv[0]);
            return 1;
        }

        if (result == NULL)
        {
            fprintf(stderr, "Error processing input.\n");
            return 1;
        }

        if (strcmp(option, "-e") == 0)
        {
            printf("Encoded message: %s\n", result);
        }
        else
        {
            printf("Decoded message: %s\n", result);
        }
        free(result);
        return 0;
    }
    else
    {
        fprintf(stderr, "Usage: %s -e|-d \"text\" OR %s -b for batch mode\n", argv[0], argv[0]);
        return 1;
    }
}