#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

#define MAX_LEN 256
#define MAX_CONTENT 1024

typedef struct Message {
    char id[37];            // UUID string in the form "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"
    char time_sent[32];     // Human-readable time string (e.g., "YYYY-MM-DD HH:MM:SS")
    char sender[MAX_LEN];   // Sender's name/text
    char receiver[MAX_LEN]; // Receiver's name/text
    char content[MAX_CONTENT]; // Message content
    int delivered;          // 0: not delivered, 1: delivered
} Message;

// Creates a new message with the given parameters.
// The time_sent field is set to the current time and delivered is initially false.
Message* create_msg(const char* id, const char* sender, const char* receiver, const char* content);

// Stores the given message on disk. Returns 0 on success.
int store_msg(Message* msg);

// Retrieves a message by its UUID (as a string) from disk.
// Returns a dynamically allocated Message* or NULL on failure.
Message* retrieve_msg(const char* id);

// Generates a UUID (version 4) and writes the result into uuid_buf (which must be at least 37 bytes).
void generate_uuid(char *uuid_buf);

#endif // MESSAGE_H