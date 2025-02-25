/**
 * encrypt.c / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "queue.h"
#include "readfile.h"

#define BATCH_SIZE 100
#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
         fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
         exit(EXIT_FAILURE);
    }

    if (access("./cipher", X_OK) == -1) {
         perror("Cipher program './cipher' not found or not executable");
         exit(EXIT_FAILURE);
    }

    queue_t *sentenceQueue = read_file_to_queue(argv[1]);
    if (!sentenceQueue) {
         fprintf(stderr, "Failed to read file into queue.\n");
         exit(EXIT_FAILURE);
    }

    if (get_queue_size(sentenceQueue) == 0) {
         fprintf(stderr, "Input file '%s' contains no sentences.\n", argv[1]);
         free(sentenceQueue);
         exit(EXIT_FAILURE);
    }

    rmdir("output");

    struct stat st = {0};
    if (stat("output", &st) == -1) {
         if (mkdir("output", 0755) == -1) {
              perror("Error creating output directory");
              exit(EXIT_FAILURE);
         }
    } 

    int batch_number = 1;
    while (get_queue_size(sentenceQueue) > 0) {
         char* batch[BATCH_SIZE];
         int count = 0;
         while (count < BATCH_SIZE && get_queue_size(sentenceQueue) > 0) {
              char* sentence = (char*) pop_queue(sentenceQueue);
              if (!sentence) {
                  break;
              }
              batch[count++] = sentence;
         }

         int pipefd[2];
         if (pipe(pipefd) == -1) {
              perror("pipe");
              exit(EXIT_FAILURE);
         }

         pid_t pid = fork();
         if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
         }
         else if (pid == 0) {
              close(pipefd[1]); 

              if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                   perror("dup2");
                   exit(EXIT_FAILURE);
              }
              close(pipefd[0]);

              char out_filename[256];
              snprintf(out_filename, sizeof(out_filename), "output/encrypted_batch_%d.txt", batch_number);

              printf("Encrypting batch %d\n", batch_number);
              // O_WRONLY: Write only
              // O_CREAT: Create the file if it doesn't exist
              // O_TRUNC: Truncate the file to zero length if it already exists
              int out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
              if (out_fd == -1) {
                   perror("open output file");
                   exit(EXIT_FAILURE);
              }
              if (dup2(out_fd, STDOUT_FILENO) == -1) {
                   perror("dup2 output file");
                   close(out_fd);
                   exit(EXIT_FAILURE);
              }
              close(out_fd);

              char *child_args[] = {"./cipher", "-b", NULL};
              if (execvp(child_args[0], child_args) == -1) {
                   perror("execvp cipher");
                   exit(EXIT_FAILURE);
              }
         }
         else {
              close(pipefd[0]);

              FILE *pipeStream = fdopen(pipefd[1], "w");
              if (pipeStream == NULL) {
                   perror("fdopen");
                   exit(EXIT_FAILURE);
              }
              for (int i = 0; i < count; i++) {
                   if (i == count - 1) {
                        fprintf(pipeStream, "%s", batch[i]);
                   } else {
                        fprintf(pipeStream, "%s\n", batch[i]);
                   }
                   free(batch[i]);
              }
              fclose(pipeStream);

              int status;
              if (waitpid(pid, &status, 0) == -1) {
                   perror("waitpid");
              }
              if (WIFEXITED(status)) {
                   int exit_status = WEXITSTATUS(status);
                   if (exit_status != 0) {
                        fprintf(stderr, "Cipher process exited with status %d for batch %d.\n", exit_status, batch_number);
                   }
              }
              else {
                   fprintf(stderr, "Cipher process did not exit normally for batch %d.\n", batch_number);
              }
         }
         batch_number++;
    }

    free(sentenceQueue);
    printf("Encryption is complete. Encrypted files are in the 'output' directory.\n");
    return 0;
} 