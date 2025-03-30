#include "message.h"
#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define MESSAGE_DIR "message_store"

// Utility function to create the message store directory if it doesn't exist.
static void ensure_directory_exists() {
    struct stat st = {0};
    if (stat(MESSAGE_DIR, &st) == -1) {
        mkdir(MESSAGE_DIR, 0700);
    }
}

// Helper function to generate a UUID version 4.
// The UUID format is "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx", where x is any hexadecimal digit and
// y is one of 8, 9, a, or b.
void generate_uuid(char *uuid_buf) {
    const char *hex_digits = "0123456789abcdef";
    int i;
    for (i = 0; i < 36; i++) {
        switch (i) {
            case 8:
            case 13:
            case 18:
            case 23:
                uuid_buf[i] = '-';
                break;
            case 14:
                uuid_buf[i] = '4';  // UUID version 4
                break;
            case 19: {
                int r = rand() % 16;
                r = (r & 0x3) | 0x8; // Set the variant bits: 10xx
                uuid_buf[i] = hex_digits[r];
                break;
            }
            default:
                uuid_buf[i] = hex_digits[rand() % 16];
                break;
        }
    }
    uuid_buf[36] = '\0';
}

// Creates a new message with the specified fields.
Message* create_msg(const char* id, const char* sender, const char* receiver, const char* content) {
    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg)
        return NULL;

    // Copy the UUID string into the message id field.
    strncpy(msg->id, id, 36);
    msg->id[36] = '\0';

    // Set the current time as a human-readable string
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(msg->time_sent, sizeof(msg->time_sent), "%Y-%m-%d %H:%M:%S", tm_info);

    strncpy(msg->sender, sender, MAX_LEN - 1);
    msg->sender[MAX_LEN - 1] = '\0';
    strncpy(msg->receiver, receiver, MAX_LEN - 1);
    msg->receiver[MAX_LEN - 1] = '\0';
    strncpy(msg->content, content, MAX_CONTENT - 1);
    msg->content[MAX_CONTENT - 1] = '\0';
    msg->delivered = 0;  // Initially not delivered
    return msg;
}

// Stores the message on disk by writing it to a binary file named "msg_<uuid>.dat"
// in the message store directory. Returns 0 on success.
// Now also stores the message in cache
int store_msg(Message* msg) {
    if (!msg)
        return -1;

    // Add to cache first
    cache_insert(msg);

    // Then write to disk
    ensure_directory_exists();

    char filepath[256];
    snprintf(filepath, sizeof(filepath), MESSAGE_DIR "/msg_%s.dat", msg->id);

    FILE* fp = fopen(filepath, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return -1;
    }

    size_t write_count = fwrite(msg, sizeof(Message), 1, fp);
    fclose(fp);

    return write_count == 1 ? 0 : -1;
}

// Retrieves the message with the given UUID from disk.
// First checks cache, then falls back to disk if not found.
// Returns a dynamically allocated Message* or NULL if not found.
// Modified retrieve_msg function to show timing and source (cache or disk)
Message* retrieve_msg(const char* id) {
    clock_t start_time = clock();  // Start timer
    
    // First check if the message is in the cache
    Message* cached_msg = cache_lookup(id);
    if (cached_msg) {
        Message* result = (Message*)malloc(sizeof(Message));
        if (!result) return NULL;
        memcpy(result, cached_msg, sizeof(Message));
        
        clock_t end_time = clock();  // End timer for cache retrieval
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Retrieved from cache in %f seconds.\n", time_taken);
        return result;
    }
    
    // Message not in cache, load from disk
    char filepath[256];
    snprintf(filepath, sizeof(filepath), MESSAGE_DIR "/msg_%s.dat", id);

    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        perror("Failed to open file for reading");
        return NULL;
    }

    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg) {
        fclose(fp);
        return NULL;
    }

    size_t read_count = fread(msg, sizeof(Message), 1, fp);
    fclose(fp);

    if (read_count != 1) {
        free(msg);
        perror("Failed to read message from file");
        return NULL;
    }
    
    // Add the loaded message to the cache
    cache_insert(msg);
    
    clock_t end_time = clock();  // End timer for disk retrieval
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Retrieved from disk in %f seconds.\n", time_taken);
    
    return msg;
}