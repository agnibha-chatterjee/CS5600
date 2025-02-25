/**
 * polybius.h / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#ifndef POLYBIUS_H
#define POLYBIUS_H

#include <stddef.h>

#define POLYBIUS_SIZE 5

typedef struct
{
    char square[POLYBIUS_SIZE][POLYBIUS_SIZE];
} polybius_table_t;

/**
 * pbEncode() encodes the plaintext string using a Polybius square cipher.
 *
 * Only alphabetical characters (A–Z) are encoded, with non‑alphabetical characters
 * are left untouched. Any 'J' is treated as 'I'.
 * Each alphabetical letter is replaced by two digits representing the row and column of
 * the letter (both 1‑indexed). For example, 'A' becomes "11".
 *
 * @param plaintext The null‑terminated input string.
 * @param table     A pointer to the polybius_table_t structure with the cipher table.
 * @return A dynamically allocated string containing the encoded text. The caller must free().
 */
char *pbEncode(const char *plaintext, const polybius_table_t *table);

/**
 * pbDecode() decodes a ciphertext string encoded with the Polybius square cipher.
 *
 * It expects pairs of digits (row and column, both 1‑indexed) for each alphabetic character.
 * Non‑digit characters are simply copied into the result. The output string is in all uppercase.
 *
 * @param ciphertext The null‑terminated encoded text.
 * @param table      A pointer to the polybius_table_t structure with the cipher table.
 * @return A dynamically allocated string containing the decoded plaintext. The caller must free().
 */
char *pbDecode(const char *ciphertext, const polybius_table_t *table);

#endif /* POLYBIUS_H */