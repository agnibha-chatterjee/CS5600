#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_usage() {
    printf("Usage:\n");
    printf("  ./app create <sender> <receiver> <message_content>\n");
    printf("    - Creates a new message with an auto-generated id and stores it.\n");
    printf("  ./app retrieve <id>\n");
    printf("    - Retrieves and displays the message with the given id.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    if (strcmp(argv[1], "create") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Insufficient arguments for create command\n");
            print_usage();
            return 1;
        }
        const char *sender = argv[2];
        const char *receiver = argv[3];
        const char *content = argv[4];
        // auto-generate id using current time
        int id = (int)time(NULL);
        Message* msg = create_msg(id, sender, receiver, content);
        if (!msg) {
            fprintf(stderr, "Failed to create message\n");
            return 1;
        }
        if (store_msg(msg) != 0) {
            fprintf(stderr, "Failed to store message\n");
            free(msg);
            return 1;
        }
        printf("Message created with auto-generated id and stored successfully.\n");
        printf("ID: %d\n", msg->id);
        printf("Time Sent: %ld\n", msg->time_sent);
        printf("Sender: %s\n", msg->sender);
        printf("Receiver: %s\n", msg->receiver);
        printf("Content: %s\n", msg->content);
        printf("Delivered: %d\n", msg->delivered);
        free(msg);
    } else if (strcmp(argv[1], "retrieve") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Insufficient arguments for retrieve command\n");
            print_usage();
            return 1;
        }
        int id = atoi(argv[2]);
        Message* msg = retrieve_msg(id);
        if (!msg) {
            fprintf(stderr, "Failed to retrieve message with id %d\n", id);
            return 1;
        }
        printf("Message Retrieved:\n");
        printf("ID: %d\n", msg->id);
        printf("Time Sent: %ld\n", msg->time_sent);
        printf("Sender: %s\n", msg->sender);
        printf("Receiver: %s\n", msg->receiver);
        printf("Content: %s\n", msg->content);
        printf("Delivered: %d\n", msg->delivered);
        free(msg);
    } else {
        fprintf(stderr, "Unrecognized command: %s\n", argv[1]);
        print_usage();
        return 1;
    }
    
    return 0;
}