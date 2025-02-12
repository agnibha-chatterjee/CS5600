/**
    queue.h / Program Execution and Process Queue
    Agnibha Chatterjee / CS5600 / Northeastern University
    Spring 2025 / Feb 11, 2025
*/

#include <stdlib.h>

// Process structure
typedef struct process {
    int identifier;
    char* name;
    long run_time;
    int priority;
} process_t;

// Internal node for the queue (doubly-linked list)
typedef struct queue_node {
    void* data;
    struct queue_node* next;
    struct queue_node* prev;
} queue_node_t;

// Queue structure
typedef struct queue {
    queue_node_t* head;
    queue_node_t* tail;
    int size;
} queue_t;

// Creates an empty queue and returns a pointer to it.
queue_t* create_queue();

// Appends element (a pointer) to the end of the queue.
void push_queue(queue_t* queue, void* element);

// Removes and returns the data pointer from the front of the queue.
// Returns NULL if the queue is empty.
void* pop_queue(queue_t* queue);

// Removes and returns the process_t pointer
// with the highest priority from the queue.
// It traverses the queue, finds the node with the highest priority,
// removes it from the list and returns its data.
process_t* remove_process(queue_t* queue);

// Returns the number of elements in the queue.
// Returns 0 if the queue is NULL or empty.
int get_queue_size(queue_t* queue);