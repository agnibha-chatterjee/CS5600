/**
 * pqueuepmain.c / Complex Synchronization
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 24, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 15

int buffer[BUF_SIZE];
int num = 0;
pthread_mutex_t mut;
pthread_cond_t cond;

int produced_total = 0;
int consumed_total = 0;
int done_producing = 0;
int done_consuming = 0;

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]) {
    pthread_t prod[2], cons[2];
    int i;

    if (pthread_mutex_init(&mut, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(1);
    }

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

    for (i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    printf("WE ARE DONE\n");
    return 0;
}


void *producer(void *param) {
    int item;
    while (1) {
        pthread_mutex_lock(&mut);

        if (produced_total >= 30) {
            if (!done_producing) {
                printf("30 items produced, exiting producers\n");
                done_producing = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond);
            pthread_exit(NULL);
        }

        while (num == BUF_SIZE && produced_total < 30) {
            pthread_cond_wait(&cond, &mut);
        }

        if (produced_total >= 30) {
            if (!done_producing) {
                printf("30 items produced, exiting producers\n");
                done_producing = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond);
            pthread_exit(NULL);
        }
        
        item = produced_total + 1;
        buffer[num] = item;
        num++;
        produced_total++;
        printf("Producer %lu produced item %d, total produced = %d, buffer count = %d\n",
               (long)pthread_self(), item, produced_total, num);

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond);
        sleep(1);
    }
}


void *consumer(void *param) {
    int item;
    while (1) {
        pthread_mutex_lock(&mut);

        if (consumed_total >= 30) {
            if (!done_consuming) {
                printf("30 items consumed, exiting consumers\n");
                done_consuming = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond); 
            pthread_exit(NULL);
        }

        while (num == 0 && consumed_total < 30) {
            pthread_cond_wait(&cond, &mut);
        }

        if (consumed_total >= 30) {
            if (!done_consuming) {
                printf("30 items consumed, exiting consumers\n");
                done_consuming = 1;
            }
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cond);
            pthread_exit(NULL);
        }

        item = buffer[num - 1];
        num--;
        consumed_total++;
        printf("Consumer %lu consumed item %d, total consumed = %d, buffer count = %d\n",
               (long)pthread_self(), item, consumed_total, num);

        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond);
        sleep(3);
    }
}
