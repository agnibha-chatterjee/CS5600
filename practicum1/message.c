#include "message.h"
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

// Creates a new message with the specified fields.
Message* create_msg(int id, const char* sender, const char* receiver, const char* content) {
    Message* msg = (Message*)malloc(sizeof(Message));
    if (!msg)
        return NULL;

    msg->id = id;
    msg->time_sent = time(NULL);
    strncpy(msg->sender, sender, MAX_LEN - 1);
    msg->sender[MAX_LEN - 1] = '\0';
    strncpy(msg->receiver, receiver, MAX_LEN - 1);
    msg->receiver[MAX_LEN - 1] = '\0';
    strncpy(msg->content, content, MAX_CONTENT - 1);
    msg->content[MAX_CONTENT - 1] = '\0';
    msg->delivered = 0;  // Initially not delivered
    return msg;
}

// Stores the message on disk by writing it to a binary file named "msg_<id>.dat"
// in the message store directory. Returns 0 on success.
int store_msg(Message* msg) {
    if (!msg)
        return -1;

    ensure_directory_exists();

    char filepath[256];
    snprintf(filepath, sizeof(filepath), MESSAGE_DIR "/msg_%d.dat", msg->id);

    FILE* fp = fopen(filepath, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return -1;
    }

    size_t write_count = fwrite(msg, sizeof(Message), 1, fp);
    fclose(fp);

    return write_count == 1 ? 0 : -1;
}

// Retrieves the message with the given id from disk. Returns NULL if not found.
Message* retrieve_msg(int id) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), MESSAGE_DIR "/msg_%d.dat", id);

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
    return msg;
}