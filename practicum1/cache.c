#include "cache.h"
#include <string.h>
#include <stdio.h>

// Global cache variables
static CacheEntry cache[CACHE_SIZE];
static int cache_initialized = 0;
static int cache_hits = 0;
static int cache_misses = 0;

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
        printf("[CACHE] Initialized with %d slots\n", CACHE_SIZE);
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

// Add a message to the cache
// If cache is full, replace the least recently used entry
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
    
    // Find an empty slot or the least recently used slot
    int lru_index = 0;
    time_t lru_time = time(NULL);
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache[i].valid) {
            // Found an empty slot
            lru_index = i;
            printf("[CACHE] Found empty slot at index %d\n", i);
            break;
        }
        
        if (cache[i].last_accessed < lru_time) {
            // Found a less recently used entry
            lru_time = cache[i].last_accessed;
            lru_index = i;
        }
    }
    
    if (cache[lru_index].valid) {
        printf("[CACHE] Replacing LRU entry in slot %d (ID: %s)\n", 
               lru_index, cache[lru_index].msg.id);
    }
    
    // Insert the message into the chosen slot
    memcpy(&cache[lru_index].msg, msg, sizeof(Message));
    cache[lru_index].valid = 1;
    cache[lru_index].last_accessed = time(NULL);
    printf("[CACHE] Message inserted in slot %d\n", lru_index);
} 