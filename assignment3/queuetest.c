/**
    queuetest.c / Program Execution and Process Queue
    Agnibha Chatterjee / CS5600 / Northeastern University
    Spring 2025 / Feb 11, 2025
*/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to print a process
void print_process(process_t* proc) {
    if(proc != NULL) {
        printf("[ID: %d, Name: %s, RunTime: %ld, Priority: %d]", 
               proc->identifier, proc->name, proc->run_time, proc->priority);
    }
}

// Helper function to print the entire queue.
void print_queue(queue_t* queue) {
    printf("Queue Size: %d\n", get_queue_size(queue));
    
    if(queue == NULL || queue->head == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    
    printf("Queue Contents: ");
    queue_node_t* current = queue->head;
    while(current != NULL) {
         process_t* proc = (process_t*) current->data;
         print_process(proc);
         printf(" -> ");
         current = current->next;
    }
    printf("NULL\n");
}

// Function to create a new process
process_t* create_process(int id, const char* name, long run_time, int priority) {
    process_t* proc = malloc(sizeof(process_t));
    if (proc == NULL) {
        fprintf(stderr, "Memory allocation error for process.\n");
        exit(EXIT_FAILURE);
    }
    proc->identifier = id;
    proc->name = strdup(name);
    proc->run_time = run_time;
    proc->priority = priority;
    return proc;
}

int main() {
    queue_t* q = create_queue();
    
    printf("Initial state: The queue should be empty.\n");
    print_queue(q);
    printf("\n");
    
    // Push several processes onto the queue
    process_t* p1 = create_process(1, "Process1", 100, 5);
    process_t* p2 = create_process(2, "Process2", 200, 3);
    process_t* p3 = create_process(3, "Process3", 150, 7);
    process_t* p4 = create_process(4, "Process4", 300, 4);
    
    printf("Pushing Process1 onto queue.\n");
    push_queue(q, p1);
    print_queue(q);
    printf("\n");
    
    printf("Pushing Process2, Process3, and Process4 onto queue.\n");
    push_queue(q, p2);
    push_queue(q, p3);
    push_queue(q, p4);
    print_queue(q);
    printf("\n");
    
    // Pop the front element (should be Process1)
    printf("Popping the front element (should be Process1)...\n");
    process_t* popped = (process_t*) pop_queue(q);
    if(popped) {
        printf("Popped: ");
        print_process(popped);
        printf("\n");
    }
    print_queue(q);
    printf("\n");
    
    // Push another process
    process_t* p5 = create_process(5, "Process5", 250, 6);
    printf("Pushing Process5 onto queue.\n");
    push_queue(q, p5);
    print_queue(q);
    printf("\n");
    
    // Remove process with highest priority (should remove Process3 with priority 7)
    printf("Removing the process with highest priority...\n");
    process_t* removed = remove_process(q);
    if(removed) {
        printf("Removed: ");
        print_process(removed);
        printf("\n");
    }
    print_queue(q);
    printf("\n");
    
    // Print the queue size
    printf("Current queue size: %d\n\n", get_queue_size(q));
    
    // Pop remaining processes
    printf("Popping all remaining processes:\n");
    while(get_queue_size(q) > 0) {
        process_t* proc = (process_t*) pop_queue(q);
        if(proc) {
            print_process(proc);
            printf("\n");
        }
    }
    print_queue(q);
    
    return 0;
} 