#include "cache.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Doubly linked list node structure
typedef struct DoublyLinkedNode {
    Message msg;                    // Message content
    char id[37];                    // Message ID for quick reference
    struct DoublyLinkedNode* prev;  // Previous node in list
    struct DoublyLinkedNode* next;  // Next node in list
} DoublyLinkedNode;

// Global cache variables
static DoublyLinkedNode* head = NULL;     // Most recently used
static DoublyLinkedNode* tail = NULL;     // Least recently used
static DoublyLinkedNode* map[CACHE_SIZE]; // Simple hashmap (indexed by slot)
static int cache_size = 0;               // Current number of entries
static int cache_hits = 0;
static int cache_misses = 0;
static CacheReplacementStrategy current_strategy = REPLACEMENT_LRU; // Default to LRU
// Added debugging function prototypes
static void print_linked_list(void);
static void print_hash_map(void);
static void print_cache_state(void);

// Initialize the cache
void init_cache() {
    // Clear any existing nodes
    DoublyLinkedNode* current = head;
    while (current != NULL) {
        DoublyLinkedNode* temp = current;
        current = current->next;
        free(temp);
    }
    
    // Reset global variables
    head = NULL;
    tail = NULL;
    cache_size = 0;
    cache_hits = 0;
    cache_misses = 0;
    
    // Clear the map
    for (int i = 0; i < CACHE_SIZE; i++) {
        map[i] = NULL;
    }
    
    printf("[CACHE] Initialized with %d slots using %s replacement\n", 
           CACHE_SIZE, get_cache_strategy_name());
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
    init_cache(); // Reuse initialization code
    printf("[CACHE] Cleared all entries\n");
}

// Get cache statistics
void get_cache_stats(int* hits, int* misses) {
    if (hits) *hits = cache_hits;
    if (misses) *misses = cache_misses;
    printf("[CACHE] Statistics: %d hits, %d misses\n", cache_hits, cache_misses);
}

// Hash function to map message ID to slot index
static int hash_function(const char* id) {
    // Simple hash function for demonstration
    unsigned long hash = 5381;
    int c;
    
    while ((c = *id++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash % CACHE_SIZE;
}

// Move a node to the front (most recently used position)
static void move_to_front(DoublyLinkedNode* node) {
    if (node == head) {
        // Already at front
        return;
    }
    
    if (node == tail) {
        // Update tail if removing the tail
        tail = node->prev;
    }
    
    // Remove from current position
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    
    // Move to front
    node->next = head;
    node->prev = NULL;
    
    if (head) head->prev = node;
    head = node;
    
    // If this was the only node, it's also the tail
    if (!tail) tail = node;
}

// Create a new node for the cache
static DoublyLinkedNode* create_node(const Message* msg) {
    DoublyLinkedNode* node = (DoublyLinkedNode*)malloc(sizeof(DoublyLinkedNode));
    if (!node) return NULL;
    
    memcpy(&node->msg, msg, sizeof(Message));
    strcpy(node->id, msg->id);
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

// Look up a message in the cache by its UUID
// Modified cache_lookup using linear probing to handle hash collisions
Message* cache_lookup(const char* id) {
    printf("[CACHE] Looking up message with ID: %s\n", id);
    
    int slot = hash_function(id);
    int start_slot = slot;
    DoublyLinkedNode* node = NULL;
    
    // Linear probing: search until an empty slot or a matching ID is found.
    while (map[slot] != NULL) {
        if (strcmp(map[slot]->id, id) == 0) {
            node = map[slot];
            break;
        }
        slot = (slot + 1) % CACHE_SIZE;
        if (slot == start_slot)
            break;
    }
    
    if (node) {
        printf("[CACHE] HIT: Message found in cache\n");
        cache_hits++;
        move_to_front(node);
        print_cache_state();  // Added: print the current cache state after retrieving
        return &node->msg;
    }
    
    // Cache miss
    printf("[CACHE] MISS: Message not found in cache\n");
    cache_misses++;
    print_cache_state();  // Added: print the current cache state after a cache miss
    return NULL;
}

// Remove the least recently used item (from tail)
// Modified remove_lru to correctly remove the node from the map using a search rather than assuming its hash location.
static void remove_lru() {
    if (!tail)
        return;
    
    printf("[CACHE] Removing LRU item: %s\n", tail->id);
    
    // Locate and clear the map entry corresponding to the tail.
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (map[i] == tail) {
            map[i] = NULL;
            break;
        }
    }
    
    // Remove node from the doubly linked list.
    DoublyLinkedNode* old_tail = tail;
    tail = tail->prev;
    
    if (tail) {
        tail->next = NULL;
    } else {
        // If the cache becomes empty, update head as well.
        head = NULL;
    }
    
    free(old_tail);
    cache_size--;
}

// Remove a random item
static void remove_random() {
    if (cache_size == 0) return;
    
    // Pick a random valid slot
    int count = 0;
    int indices[CACHE_SIZE];
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (map[i] != NULL) {
            indices[count++] = i;
        }
    }
    
    if (count == 0) return;
    
    int random_index = rand() % count;
    int slot = indices[random_index];
    DoublyLinkedNode* node = map[slot];
    
    printf("[CACHE] Removing random item: %s\n", node->id);
    
    // Remove from linked list
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    
    // Update head/tail if needed
    if (node == head) head = node->next;
    if (node == tail) tail = node->prev;
    
    // Clear map entry
    map[slot] = NULL;
    
    free(node);
    cache_size--;
}

// Add a message to the cache
// Modified cache_insert to use linear probing when checking/inserting cache entries
void cache_insert(const Message* msg) {
    if (!msg)
        return;
    
    printf("[CACHE] Inserting message with ID: %s\n", msg->id);
    
    int slot = hash_function(msg->id);
    int start_slot = slot;
    
    // Check if the message is already in the cache using linear probing.
    while (map[slot] != NULL) {
        if (strcmp(map[slot]->id, msg->id) == 0) {
            // Update existing entry.
            memcpy(&map[slot]->msg, msg, sizeof(Message));
            move_to_front(map[slot]);
            printf("[CACHE] Updated existing entry\n");
            print_cache_state();  // Added: print the current cache state after updating an entry
            return;
        }
        slot = (slot + 1) % CACHE_SIZE;
        if (slot == start_slot)
            break;
    }
    
    // Make room if needed.
    if (cache_size >= CACHE_SIZE) {
        if (current_strategy == REPLACEMENT_LRU) {
            remove_lru();
        } else {
            remove_random();
        }
        // After eviction, find an empty slot.
        slot = hash_function(msg->id);
        start_slot = slot;
        while (map[slot] != NULL) {
            slot = (slot + 1) % CACHE_SIZE;
            if (slot == start_slot)
                break;
        }
    }
    
    // Create new node.
    DoublyLinkedNode* new_node = create_node(msg);
    if (!new_node) {
        printf("[CACHE] ERROR: Failed to allocate memory for new cache entry\n");
        return;
    }
    
    // Add the new node to the front (most recently used).
    new_node->next = head;
    if (head)
        head->prev = new_node;
    head = new_node;
    if (!tail)
        tail = new_node;
    
    // Insert into the map at the found empty slot.
    map[slot] = new_node;
    cache_size++;
    
    printf("[CACHE] Message inserted in cache\n");
    print_cache_state();  // Added: print the current cache state after insertion
}

// Added debugging function definitions
static void print_linked_list(void) {
    printf("[CACHE] Linked List: ");
    DoublyLinkedNode* current = head;
    while (current != NULL) {
        printf("%s -> ", current->id);
        current = current->next;
    }
    printf("NULL\n");
}

static void print_hash_map(void) {
    printf("[CACHE] Hash Map:\n");
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (map[i] != NULL) {
            printf("  Slot %d: %s\n", i, map[i]->id);
        } else {
            printf("  Slot %d: empty\n", i);
        }
    }
}

static void print_cache_state(void) {
    print_linked_list();
    print_hash_map();
}