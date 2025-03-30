#include "message.h"
#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define NUM_TESTS 1000  // Number of random message accesses for evaluation

// Ensure the message store directory exists
static void ensure_message_store_exists() {
    struct stat st = {0};
    if (stat("message_store", &st) == -1) {
        printf("Creating message_store directory...\n");
        mkdir("message_store", 0700);
    }
}

// Function to evaluate cache performance using NUM_TESTS random message accesses
void evaluate_cache_performance(CacheReplacementStrategy strategy) {
    // Initialize cache with the specified strategy
    init_cache();
    set_cache_strategy(strategy);
    clear_cache();
    
    printf("\n---- EVALUATING CACHE WITH %s STRATEGY ----\n", get_cache_strategy_name());
    
    // Ensure message store directory exists
    ensure_message_store_exists();
    
    // Create some initial messages to work with (more than cache size)
    const int num_messages = CACHE_SIZE + 10;
    char **ids = (char **)malloc(num_messages * sizeof(char *));
    
    // Generate and store messages
    printf("Creating %d test messages...\n", num_messages);
    for (int i = 0; i < num_messages; i++) {
        ids[i] = (char *)malloc(37 * sizeof(char));
        generate_uuid(ids[i]);
        
        char sender[20], receiver[20], content[50];
        sprintf(sender, "sender%d", i);
        sprintf(receiver, "receiver%d", i);
        sprintf(content, "This is test message content %d", i);
        
        Message *msg = create_msg(ids[i], sender, receiver, content);
        store_msg(msg);

        free(msg);

        clear_cache();

    }
    
    // Reset cache stats before evaluation
    // clear_cache();
    
    printf("Beginning evaluation with %d random accesses...\n", NUM_TESTS);
    
    // Perform random message accesses
    for (int i = 0; i < NUM_TESTS; i++) {
        // Randomly choose between existing messages and non-existent ones
        if (rand() % 100 < 70) {  // 70% chance to access existing message
            int msg_index = rand() % num_messages;
            Message *msg = retrieve_msg(ids[msg_index]);
            if (msg) free(msg);
        } else {  // 30% chance to try a non-existent message
            char random_id[37];
            generate_uuid(random_id);
            Message *msg = retrieve_msg(random_id);
            if (msg) free(msg);  // Shouldn't happen, but just in case
        }
    }
    
    // Get cache statistics
    int hits, misses;
    get_cache_stats(&hits, &misses);
    
    // Calculate metrics
    double hit_ratio = (double)hits / (hits + misses);
    
    // Print results
    printf("\nCACHE EVALUATION RESULTS (%s):\n", get_cache_strategy_name());
    printf("Number of cache hits per %d accesses: %d\n", NUM_TESTS, hits);
    printf("Number of cache misses per %d accesses: %d\n", NUM_TESTS, misses);
    printf("Cache hit ratio: %.2f\n", hit_ratio);
    
    // Clean up
    for (int i = 0; i < num_messages; i++) {
        free(ids[i]);
    }
    free(ids);
}

// Main function to run the evaluation
int main(void) {
    // Seed the random number generator
    srand((unsigned)time(NULL));
    
    printf("CACHE EVALUATION - TESTING PERFORMANCE WITH %d RANDOM ACCESSES\n", NUM_TESTS);
    printf("============================================================\n");
    
    // Test LRU strategy
    evaluate_cache_performance(REPLACEMENT_LRU);
    
    // Test Random strategy
    evaluate_cache_performance(REPLACEMENT_RANDOM);
    
    return 0;
} 