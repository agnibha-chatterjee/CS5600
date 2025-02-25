/**
 * readfile.c / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_LINE_LENGTH 1024


queue_t *read_file_to_queue(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
         perror("Error opening file");
         return NULL;
    }

    queue_t *queue = create_queue();
    if (!queue) {
         fclose(fp);
         return NULL;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
         size_t len = strlen(buffer);
         if (len > 0 && buffer[len-1] == '\n')
              buffer[len-1] = '\0';

         char *line = strdup(buffer);
         if (!line) {
              fprintf(stderr, "Memory allocation error.\n");
              fclose(fp);
              return NULL;
         }
         push_queue(queue, line);
    }
    fclose(fp);
    return queue;
}

#ifdef DEMO_READFILE_MAIN

int main(int argc, char *argv[]) {
    if (argc < 2) {
         fprintf(stderr, "Usage: %s <input_filename>\n", argv[0]);
         return 1;
    }

    queue_t *queue = read_file_to_queue(argv[1]);
    if (!queue) {
         fprintf(stderr, "Failed to read file into queue.\n");
         return 1;
    }

    printf("Lines from file:\n");
    while (get_queue_size(queue) > 0) {
         char *line = (char *)pop_queue(queue);
         if (line) {
              printf("%s\n", line);
              free(line);
         }
    }
    free(queue);
    return 0;
}
#endif 