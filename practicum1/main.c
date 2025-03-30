#include "message.h"
#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   
#include <unistd.h>   

void print_usage() {
    printf("Usage:\n");
    printf("  ./app create <sender> <receiver> <message_content> [--strategy=lru|random]\n");
    printf("    - Creates a new message with an auto-generated UUID and stores it.\n");
    printf("  ./app store <uuid> <sender> <receiver> <message_content> [--strategy=lru|random]\n");
    printf("    - Creates a new message with the specified UUID and stores it.\n");
    printf("  ./app retrieve <uuid> [--strategy=lru|random]\n");
    printf("    - Retrieves and displays the message with the given UUID.\n");
    printf("  ./app delete all [--strategy=lru|random]\n");
    printf("    - Clears the contents of the message_store folder.\n");
    printf("  ./app cache-stats\n");
    printf("    - Displays cache hit/miss statistics.\n");
    printf("\nOptions:\n");
    printf("  --strategy=lru     Use LRU (Least Recently Used) cache replacement strategy\n");
    printf("  --strategy=random  Use Random cache replacement strategy\n");
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

// Parse command line arguments for strategy option
// Returns the index of the strategy argument if found, or -1 if not found
int parse_strategy_arg(int argc, char *argv[], CacheReplacementStrategy *strategy) {
    const char *lru_str = "--strategy=lru";
    const char *random_str = "--strategy=random";
    
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--strategy=", 11) == 0) {
            if (strcmp(argv[i], lru_str) == 0) {
                *strategy = REPLACEMENT_LRU;
                printf("Using LRU cache replacement strategy\n");
                return i;
            } else if (strcmp(argv[i], random_str) == 0) {
                *strategy = REPLACEMENT_RANDOM;
                printf("Using Random cache replacement strategy\n");
                return i;
            } else {
                fprintf(stderr, "Unknown strategy format: %s\n", argv[i]);
                printf("Available strategies: --strategy=lru, --strategy=random\n");
                return -2; // Error code for unknown strategy
            }
        }
    }
    
    return -1; // Strategy not specified
}

int main(int argc, char *argv[]) {
    // Seed the random number generator for UUID generation and random replacement
    srand((unsigned) time(NULL));
    
    // Initialize the cache
    init_cache();

    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    // Parse strategy option if present
    CacheReplacementStrategy strategy;
    int strategy_index = parse_strategy_arg(argc, argv, &strategy);
    
    // Set strategy if specified and valid
    if (strategy_index > 0) {
        set_cache_strategy(strategy);
    } else if (strategy_index == -2) {
        // Invalid strategy format
        return 1;
    }
    
    if (strcmp(argv[1], "cache-stats") == 0) {
        int hits, misses;
        get_cache_stats(&hits, &misses);
        printf("Current cache replacement strategy: %s\n", get_cache_strategy_name());
        return 0;
    } else if (strcmp(argv[1], "create") == 0) {
        int required_args = 5;  // Including the command itself
        if (strategy_index > 0) required_args--;
        
        if (argc < required_args) {
            fprintf(stderr, "Insufficient arguments for create command\n");
            print_usage();
            return 1;
        }
        
        int sender_idx = 2;
        int receiver_idx = 3;
        int content_idx = 4;
        
        const char *sender = argv[sender_idx];
        const char *receiver = argv[receiver_idx];
        const char *content = argv[content_idx];

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
        int required_args = 6;  // Including the command itself
        if (strategy_index > 0) required_args--;
        
        if (argc < required_args) {
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
    } else if (strcmp(argv[1], "retrieve") == 0) {
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
        char uuid4[37];
        char uuid5[37];

        generate_uuid(uuid1);
        generate_uuid(uuid2);
        generate_uuid(uuid3);
        generate_uuid(uuid4);
        generate_uuid(uuid5);

        Message* msg1 = create_msg(uuid1, "sender1", "receiver1", "content1");
        Message* msg2 = create_msg(uuid2, "sender2", "receiver2", "content2");
        Message* msg3 = create_msg(uuid3, "sender3", "receiver3", "content3");
        Message* msg4 = create_msg(uuid4, "sender4", "receiver4", "content4");
        Message* msg5 = create_msg(uuid5, "sender5", "receiver5", "content5");

        printf("Storing messages...\n");
        store_msg(msg1);
        store_msg(msg2);
        store_msg(msg3);
        store_msg(msg4);
        store_msg(msg5);

        printf("Retrieving messages...\n");
        retrieve_msg(uuid1);
        retrieve_msg(uuid1);
        // retrieve_msg(uuid3);
        // retrieve_msg(uuid4);
        // retrieve_msg(uuid5);

        free(msg1);
        free(msg2);
        free(msg3);
        free(msg4);
        free(msg5);
    } else {
        fprintf(stderr, "Unrecognized command: %s\n", argv[1]);
        print_usage();
        return 1;
    }
    
    return 0;
}