/**
    queue.c / Program Execution and Process Queue
    Agnibha Chatterjee / CS5600 / Northeastern University
    Spring 2025 / Feb 11, 2025
*/

#include "queue.h"
#include <stdio.h>

// This function creates a new queue
queue_t* create_queue() {
    queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
    if (!queue) {
        fprintf(stderr, "Error allocating memory for queue.\n");
        exit(EXIT_FAILURE);
    }
    queue->head = queue->tail = NULL;
    queue->size = 0;
    return queue;
}

// This function creates a new node containing the given element,
// and appends it to the end of the queue.
void push_queue(queue_t* queue, void* element) {
    if (queue == NULL)
        return;
    queue_node_t* node = (queue_node_t*) malloc(sizeof(queue_node_t));
    if (node == NULL) {
         fprintf(stderr, "Memory allocation error in push_queue.\n");
         exit(EXIT_FAILURE);
    }
    node->data = element;
    node->next = NULL;
    node->prev = queue->tail;
    
    if(queue->tail != NULL)
         queue->tail->next = node;
    
    queue->tail = node;
    
    if(queue->head == NULL)
         queue->head = node;
    
    queue->size++;
}

// This function removes the node from the front of the queue,
// returns the stored data pointer.
void* pop_queue(queue_t* queue) {
    if(queue == NULL || queue->head == NULL)
        return NULL;
    
    queue_node_t* node = queue->head;
    void* data = node->data;
    
    queue->head = node->next;
    if(queue->head != NULL)
         queue->head->prev = NULL;
    else
         queue->tail = NULL; // Queue becomes empty
    
    queue->size--;
    free(node);
    return data;
}

// This function traverses the queue to find the process_t with the
// highest priority (higher the number, higher the priority), removes that node from the queue,
// and returns the process_t pointer contained in it.
process_t* remove_process(queue_t* queue) {
    if(queue == NULL || queue->head == NULL)
         return NULL;
    
    queue_node_t* current = queue->head;
    queue_node_t* highest_node = current;
    
    while(current != NULL) {
         process_t* proc = (process_t*)(current->data);
         process_t* highest_proc = (process_t*)(highest_node->data);
         if(proc->priority > highest_proc->priority) {
             highest_node = current;
         }
         current = current->next;
    }
    
    // Remove highest_node from the doubly linked list
    if(highest_node->prev != NULL)
         highest_node->prev->next = highest_node->next;
    else // Removing head
         queue->head = highest_node->next;
    
    if(highest_node->next != NULL)
         highest_node->next->prev = highest_node->prev;
    else // Removing tail
         queue->tail = highest_node->prev;
    
    process_t* data = (process_t*) highest_node->data;
    free(highest_node);
    queue->size--;
    
    return data;
}

// This function returns the current number of elements in the queue.
int get_queue_size(queue_t* queue) {
    if(queue == NULL)
         return 0;
    return queue->size;
} 