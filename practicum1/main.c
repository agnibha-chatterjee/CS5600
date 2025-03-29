#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   
#include <unistd.h>   

void print_usage() {
    printf("Usage:\n");
    printf("  ./app create <sender> <receiver> <message_content>\n");
    printf("    - Creates a new message with an auto-generated UUID and stores it.\n");
    printf("  ./app store <uuid> <sender> <receiver> <message_content>\n");
    printf("    - Creates a new message with the specified UUID and stores it.\n");
    printf("  ./app retrieve <uuid>\n");
    printf("    - Retrieves and displays the message with the given UUID.\n");
    printf("  ./app delete all\n");
    printf("    - Clears the contents of the message_store folder.\n");
}

// Helper function to delete all files in the message_store directory.
int delete_all_messages() {
    const char *dir = "message_store";
    DIR *d = opendir(dir);
    if (d == NULL) {
        perror("Unable to open message_store directory");
        return -1;
    }
    struct dirent *entry;
    int count = 0;
    char filepath[512];
    while ((entry = readdir(d)) != NULL) {
        // Skip the "." and ".." entries.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(filepath, sizeof(filepath), "%s/%s", dir, entry->d_name);
        if (remove(filepath) == 0) {
            count++;
        } else {
            perror("Failed to delete file");
        }
    }
    closedir(d);
    return count;
}

int main(int argc, char *argv[]) {
    // Seed the random number generator for UUID generation.
    srand((unsigned) time(NULL));

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

        char uuid[37];
        generate_uuid(uuid);

        Message* msg = create_msg(uuid, sender, receiver, content);
        if (!msg) {
            fprintf(stderr, "Failed to create message\n");
            return 1;
        }
        if (store_msg(msg) != 0) {
            fprintf(stderr, "Failed to store message\n");
            free(msg);
            return 1;
        }
        printf("Message created with auto-generated UUID and stored successfully.\n");
        printf("Message ID: %s\n", msg->id);
        printf("Time Sent: %s\n", msg->time_sent);
        printf("Sender: %s\n", msg->sender);
        printf("Receiver: %s\n", msg->receiver);
        printf("Content: %s\n", msg->content);
        printf("Delivered: %d\n", msg->delivered);
        free(msg);
    } else if (strcmp(argv[1], "store") == 0) {
        if (argc < 6) {
            fprintf(stderr, "Insufficient arguments for store command\n");
            print_usage();
            return 1;
        }
        const char *uuid = argv[2];
        const char *sender = argv[3];
        const char *receiver = argv[4];
        const char *content = argv[5];
        Message* msg = create_msg(uuid, sender, receiver, content);
        if (!msg) {
            fprintf(stderr, "Failed to create message\n");
            return 1;
        }
        if (store_msg(msg) != 0) {
            fprintf(stderr, "Failed to store message\n");
            free(msg);
            return 1;
        }
        printf("Message with UUID %s stored successfully.\n", msg->id);
        free(msg);
    }  else if (strcmp(argv[1], "retrieve") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Insufficient arguments for retrieve command\n");
            print_usage();
            return 1;
        }
        const char *uuid = argv[2];
        Message* msg = retrieve_msg(uuid);
        if (!msg) {
            fprintf(stderr, "Failed to retrieve message with UUID %s\n", uuid);
            return 1;
        }
        printf("Message Retrieved:\n");
        printf("Message ID: %s\n", msg->id);
        printf("Time Sent: %s\n", msg->time_sent);
        printf("Sender: %s\n", msg->sender);
        printf("Receiver: %s\n", msg->receiver);
        printf("Content: %s\n", msg->content);
        printf("Delivered: %d\n", msg->delivered);
        free(msg);
    } else if (strcmp(argv[1], "delete") == 0) {
        if (argc >= 3 && strcmp(argv[2], "all") == 0) {
            int deleted = delete_all_messages();
            if (deleted < 0) {
                fprintf(stderr, "Error occurred while deleting messages.\n");
                return 1;
            }
            printf("Deleted %d messages from the message_store folder.\n", deleted);
        } else {
            fprintf(stderr, "Invalid delete command. Usage: ./app delete all\n");
            print_usage();
            return 1;
        }
    } else if (strcmp(argv[1], "simulate") == 0) {
        char uuid1[37];
        char uuid2[37];
        char uuid3[37];
        generate_uuid(uuid1);
        generate_uuid(uuid2);
        generate_uuid(uuid3);

        Message* msg1 = create_msg(uuid1, "sender1", "receiver1", "content1");
        Message* msg2 = create_msg(uuid2, "sender2", "receiver2", "content2");
        Message* msg3 = create_msg(uuid3, "sender3", "receiver3", "content3");
    
        store_msg(msg1);
        store_msg(msg2);
        store_msg(msg3);

        free(msg1);
        free(msg2);
        free(msg3);
    } else {
        fprintf(stderr, "Unrecognized command: %s\n", argv[1]);
        print_usage();
        return 1;
    }
    
    return 0;
}