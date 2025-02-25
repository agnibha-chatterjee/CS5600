/**
 * genrand.c / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned long lcg_state;

void seedGenerator()
{
    lcg_state = (unsigned long)time(NULL);
}


int genRand(int lower, int upper)
{
    static const unsigned long a = 1122334455UL;
    static const unsigned long c = 12345UL;
    static const unsigned long m = 1UL << 31; 

    lcg_state = (a * lcg_state + c) % m;

    int range = upper - lower + 1;
    return lower + (int)(lcg_state % range);
}


void genRandString(char *dest, int length)
{
    for (int i = 0; i < length; i++) {
        int idx = genRand(0, 25);
        dest[i] = 'a' + idx;
    }
    dest[length] = '\0';
}

int main(int argc, char *argv[])

{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <number_of_strings> <filename> [-a]\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);
    if (count <= 0)
    {
        fprintf(stderr, "Error: number_of_strings must be a positive integer.\n");
        return 1;
    }

    if (count < 10000) {
        count = 10000;
    }

    const char *filename = argv[2];
    FILE *file;

    if (argc >= 4 && strcmp(argv[3], "-a") == 0)
    {
        file = fopen(filename, "a");
    }
    else
    {
        file = fopen(filename, "w");
    }

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    seedGenerator();

    char buffer[11];
    for (int i = 0; i < count; i++)
    {
        int str_length = genRand(5, 10);
        genRandString(buffer, str_length);
        if (i == count - 1) {
            // Skip newline for last string
            fprintf(file, "%s", buffer);
        } else {
            fprintf(file, "%s\n", buffer);
        }
    }

    fclose(file);
    return 0;
}