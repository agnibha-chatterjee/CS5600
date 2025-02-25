/**
 * pqueuepmain.c / Complex Synchronization
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 24, 2025
 *
 * Modifications:
 * - Increased buffer capacity from 10 to 15
 * - Created at least 2 producer threads and 2 consumer threads
 * - Added counters to track the total items produced and consumed
 * - Added exit conditions: once 30 items have been produced or consumed,
 *   print a message and exit the corresponding threads
 * - Inserted extra debug print messages for clarity
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 15   // Modified: Bumped up buffer capacity to 15

int buffer[BUF_SIZE];
int num = 0;
pthread_mutex_t mut;
pthread_cond_t cond;

int produced_total = 0;   // Counter for total items produced
int consumed_total = 0;   // Counter for total items consumed
int done_producing = 0;   // Flag to indicate producers have exited
int done_consuming = 0;   // Flag to indicate consumers have exited

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]) {
    pthread_t prod[2], cons[2]; // Array for at least 2 producers and 2 consumers
    int i;

    // Initialize mutex and condition variable
    if (pthread_mutex_init(&mut, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);
    }

    // Create 2 producer and 2 consumer threads
    for(i = 0; i < 2; i++) {
        if (pthread_create(&prod[i], NULL, producer, NULL) != 0) {
            perror("pthread_create producer");
            exit(1);
        }
        if (pthread_create(&cons[i], NULL, consumer, NULL) != 0) {
            perror("pthread_create consumer");
            exit(1);
        }
    }

    // Wait for all producer and consumer threads to finish
    for (i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    
    // Clean up mutex and condition variable resources
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    printf("WE ARE DONE\n");
    return 0;
}


void *producer(void *param) {
    int item;
    while (1) {
        pthread_mutex_lock(&mut);

        // Exit condition: if 30 items have been produced, exit producer thread
        if (produced_total >= 30) {
            if (!done_producing) {
                printf("30 items produced, exiting producers\n");
                done_producing = 1; // Mark that producers have exited
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond); // Notify others to re-check exit condition
            pthread_exit(NULL);
        }

        // Wait if the buffer is full and we haven't produced 30 items yet
        while (num == BUF_SIZE && produced_total < 30) {
            pthread_cond_wait(&cond, &mut);
        }

        // Double-check condition after waking up (to handle spurious wake-ups)
        if (produced_total >= 30) {
            if (!done_producing) {
                printf("30 items produced, exiting producers\n");
                done_producing = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond);
            pthread_exit(NULL);
        }
        
        // Produce an item and add it to the buffer
        item = produced_total + 1;
        buffer[num] = item;
        num++;
        produced_total++;
        printf("Producer %lu produced item %d, total produced = %d, buffer count = %d\n",
               (long)pthread_self(), item, produced_total, num);

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond);  // Wake up waiting threads
        sleep(1); // Delay to simulate production time
    }
}


void *consumer(void *param) {
    int item;
    while (1) {
        pthread_mutex_lock(&mut);

        // Exit condition: if 30 items have been consumed, exit consumer thread
        if (consumed_total >= 30) {
            if (!done_consuming) {
                printf("30 items consumed, exiting consumers\n");
                done_consuming = 1; // Mark that consumers have exited
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond); 
            pthread_exit(NULL);
        }

        // Wait if the buffer is empty and we haven't consumed 30 items yet
        while (num == 0 && consumed_total < 30) {
            pthread_cond_wait(&cond, &mut);
        }

        // Double-check condition after waking up (to handle spurious wake-ups)
        if (consumed_total >= 30) {
            if (!done_consuming) {
                printf("30 items consumed, exiting consumers\n");
                done_consuming = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond);
            pthread_exit(NULL);
        }

        // Consume an item from the buffer
        item = buffer[num - 1];
        num--;
        consumed_total++;
        printf("Consumer %lu consumed item %d, total consumed = %d, buffer count = %d\n",
               (long)pthread_self(), item, consumed_total, num);

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond);  // Wake up waiting threads
        sleep(3); // Delay to simulate consumption time
    }
}
