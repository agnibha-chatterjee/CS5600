/**
 * queue.h / Multiprocessing in C
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 22, 2025
 */

#ifndef QUEUE_H
#define QUEUE_H

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

/**
 * create_queue() creates an empty queue and returns a pointer to it.
 * @return A pointer to the created queue.
 */
queue_t* create_queue();

/**
 * push_queue() appends an element to the end of the queue.
 * @param queue The queue to append the element to.
 * @param element The element to append.
 */
void push_queue(queue_t* queue, void* element);

/**
 * pop_queue() removes and returns the data pointer from the front of the queue.
 * @param queue The queue to pop the element from.
 * @return The data pointer from the front of the queue.
 */
void* pop_queue(queue_t* queue);

/**
 * remove_process() removes and returns the process_t pointer with the highest priority from the queue.
 * It traverses the queue, finds the node with the highest priority, removes it from the list and returns its data.
 * @param queue The queue to remove the process from.
 * @return The process_t pointer with the highest priority.
 */
process_t* remove_process(queue_t* queue);

/**
 * get_queue_size() returns the number of elements in the queue.
 * @param queue The queue to get the size of.
 * @return The number of elements in the queue.
 */
int get_queue_size(queue_t* queue);

#endif