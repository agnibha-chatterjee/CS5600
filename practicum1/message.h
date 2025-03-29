#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

#define MAX_LEN 256
#define MAX_CONTENT 1024

typedef struct Message {
    int id;                         // Unique identifier
    time_t time_sent;               // Time the message was sent
    char sender[MAX_LEN];           // Sender's name/text
    char receiver[MAX_LEN];         // Receiver's name/text
    char content[MAX_CONTENT];      // Message contents (text)
    int delivered;                  // 0: not delivered, 1: delivered
} Message;

// Creates a new message with the given parameters. 
// The time_sent field is set to the current time and delivered is initially false.
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);

// Stores the given message on disk. This implementation stores each message
// as a separate binary file under a designated directory. Returns 0 on success.
int store_msg(Message* msg);

// Retrieves a message by its identifier from disk. Returns a dynamically allocated Message*,
// or NULL on failure.
Message* retrieve_msg(int id);

#endif // MESSAGE_H