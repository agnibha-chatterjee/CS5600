#ifndef CACHE_H
#define CACHE_H

#include "message.h"
#include <time.h>

// Cache configuration
#define CACHE_SIZE 16     // Number of messages in cache

// Cache replacement strategies
typedef enum {
    REPLACEMENT_LRU,     // Least Recently Used replacement
    REPLACEMENT_RANDOM   // Random replacement
} CacheReplacementStrategy;

// Cache entry structure
typedef struct CacheEntry {
    Message msg;           // The cached message
    time_t last_accessed;  // Last time this entry was accessed
    int valid;             // 1 if entry contains valid data, 0 otherwise
} CacheEntry;

// Initialize the message cache
void init_cache();

// Set the cache replacement strategy
void set_cache_strategy(CacheReplacementStrategy strategy);

// Clear the cache (mark all entries as invalid)
void clear_cache();

// Look up a message in the cache by its UUID
// Returns pointer to message in cache if found, NULL otherwise
Message* cache_lookup(const char* id);

// Add a message to the cache
// If cache is full, replace an entry based on the current replacement strategy
void cache_insert(const Message* msg);

// Get cache statistics (hits, misses)
void get_cache_stats(int* hits, int* misses);

// Get a string representation of the current replacement strategy
const char* get_cache_strategy_name();

#endif // CACHE_H 