#include "cache.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Global cache variables
static CacheEntry cache[CACHE_SIZE];
static int cache_initialized = 0;
static int cache_hits = 0;
static int cache_misses = 0;
static CacheReplacementStrategy current_strategy = REPLACEMENT_LRU; // Default to LRU

// Initialize the cache
void init_cache() {
    if (!cache_initialized) {
        for (int i = 0; i < CACHE_SIZE; i++) {
            cache[i].valid = 0;
            cache[i].last_accessed = 0;
        }
        cache_initialized = 1;
        cache_hits = 0;
        cache_misses = 0;
        printf("[CACHE] Initialized with %d slots using %s replacement\n", 
               CACHE_SIZE, get_cache_strategy_name());
    }
}

// Set the cache replacement strategy
void set_cache_strategy(CacheReplacementStrategy strategy) {
    current_strategy = strategy;
    printf("[CACHE] Replacement strategy set to: %s\n", get_cache_strategy_name());
}

// Get a string representation of the current replacement strategy
const char* get_cache_strategy_name() {
    switch (current_strategy) {
        case REPLACEMENT_LRU:
            return "LRU";
        case REPLACEMENT_RANDOM:
            return "Random";
        default:
            return "Unknown";
    }
}

// Clear the cache
void clear_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].valid = 0;
    }
    cache_hits = 0;
    cache_misses = 0;
    printf("[CACHE] Cleared all entries\n");
}

// Get cache statistics
void get_cache_stats(int* hits, int* misses) {
    if (hits) *hits = cache_hits;
    if (misses) *misses = cache_misses;
    printf("[CACHE] Statistics: %d hits, %d misses\n", cache_hits, cache_misses);
}

// Look up a message in the cache by its UUID
// Returns NULL if not found in cache
Message* cache_lookup(const char* id) {
    if (!cache_initialized) init_cache();
    
    printf("[CACHE] Looking up message with ID: %s\n", id);
    
    // Search for the message in the cache
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].valid && strcmp(cache[i].msg.id, id) == 0) {
            // Update access time
            cache[i].last_accessed = time(NULL);
            cache_hits++;
            printf("[CACHE] HIT: Message found in cache slot %d\n", i);
            return &cache[i].msg;
        }
    }
    
    cache_misses++;
    printf("[CACHE] MISS: Message not found in cache\n");
    return NULL;
}

// Find an empty slot or return -1 if cache is full
static int find_empty_slot() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache[i].valid) {
            return i;
        }
    }
    return -1; // No empty slots
}

// Find the least recently used slot
static int find_lru_slot() {
    int lru_index = 0;
    time_t lru_time = cache[0].last_accessed;
    
    for (int i = 1; i < CACHE_SIZE; i++) {
        if (cache[i].last_accessed < lru_time) {
            lru_time = cache[i].last_accessed;
            lru_index = i;
        }
    }
    
    return lru_index;
}

// Find a random slot
static int find_random_slot() {
    return rand() % CACHE_SIZE;
}

// Add a message to the cache
// If cache is full, replace an entry based on the current replacement strategy
void cache_insert(const Message* msg) {
    if (!cache_initialized) init_cache();
    if (!msg) return;
    
    printf("[CACHE] Inserting message with ID: %s\n", msg->id);
    
    // First check if this message is already in the cache
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].valid && strcmp(cache[i].msg.id, msg->id) == 0) {
            // Update existing entry
            memcpy(&cache[i].msg, msg, sizeof(Message));
            cache[i].last_accessed = time(NULL);
            printf("[CACHE] Updated existing entry in slot %d\n", i);
            return;
        }
    }
    
    // Find an empty slot if available
    int slot_index = find_empty_slot();
    
    // If no empty slot, use the selected replacement strategy
    if (slot_index == -1) {
        switch (current_strategy) {
            case REPLACEMENT_LRU:
                slot_index = find_lru_slot();
                printf("[CACHE] Using LRU replacement strategy\n");
                break;
                
            case REPLACEMENT_RANDOM:
                slot_index = find_random_slot();
                printf("[CACHE] Using Random replacement strategy\n");
                break;
                
            default:
                // Fallback to LRU if unknown strategy
                slot_index = find_lru_slot();
                printf("[CACHE] Using LRU replacement strategy (fallback)\n");
                break;
        }
        
        printf("[CACHE] Replacing entry in slot %d (ID: %s)\n", 
               slot_index, cache[slot_index].msg.id);
    } else {
        printf("[CACHE] Found empty slot at index %d\n", slot_index);
    }
    
    // Insert the message into the chosen slot
    memcpy(&cache[slot_index].msg, msg, sizeof(Message));
    cache[slot_index].valid = 1;
    cache[slot_index].last_accessed = time(NULL);
    printf("[CACHE] Message inserted in slot %d\n", slot_index);
} 