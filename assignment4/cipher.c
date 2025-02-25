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

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Usage: %s -e|-d \"text\" OR %s -b for batch mode\n", argv[0], argv[0]);
        return 1;
    }

    char *option = argv[1];

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

    // Handle batch mode
    if (strcmp(option, "-b") == 0)
    {
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), stdin) != NULL)
        {
            // Remove newline character if present
            size_t len = strlen(line);
            if (len > 0 && line[len-1] == '\n')
                line[len-1] = '\0';
            
            // Encode the line
            char *result = pbEncode(line, &table);
            if (result)
            {
                printf("%s\n", result);
                free(result);
            }
            else
            {
                fprintf(stderr, "Error encoding line: %s\n", line);
            }
        }
        return 0;
    }
    
    // Handle single text mode
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s -e|-d \"text\" OR %s -b for batch mode\n", argv[0], argv[0]);
        return 1;
    }

    char *input = argv[2];
    char *result = NULL;
    const char *mode = NULL;

    // Determine mode and perform the operation
    if (strcmp(option, "-e") == 0)
    {
        result = pbEncode(input, &table);
        mode = "Encoded";
    }
    else if (strcmp(option, "-d") == 0)
    {
        result = pbDecode(input, &table);
        mode = "Decoded";
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

    printf("%s message: %s\n", mode, result);
    free(result);
    return 0;
}