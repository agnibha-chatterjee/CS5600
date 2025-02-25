/**
 * readfile.h / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#ifndef READFILE_H
#define READFILE_H

#include "queue.h"

/**
 * read_file_to_queue() reads an input file line-by-line, 
 * removes trailing newline characters, and pushes each line into a queue.
 * @param filename The name of the input file to read.
 * @return A pointer to the populated queue, or NULL on error.
 */
queue_t *read_file_to_queue(const char *filename);

#endif