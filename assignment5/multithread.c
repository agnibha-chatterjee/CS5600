/**
 * multithread.c / Complex Synchronization
 * Agnibha Chatterjee / CS5600 / Northeastern University
 * Spring 2025 / Feb 24, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int global_array[10];

void *worker(void *data)
{
    char *name = (char*)data;
    
    if(strcmp(name, "X") == 0)
    {
        int old_state;
        
        for (int j = 0; j < 10; j++) {
            global_array[j] = (j + 1) * 100;
        }
        printf("Thread %s wrote global data.\n", name);
        
    }
    else if(strcmp(name, "Y") == 0)
    {
        sleep(1);
        
        int old_state;
        
        printf("Thread %s reading global array: ", name);
        for (int j = 0; j < 10; j++) {
            printf("%d ", global_array[j]);
        }
        printf("\n");
        
    }
    
    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        printf("Hi from thread name = %s\n", name);
    }
 
    printf("Thread %s done!\n", name);
    return NULL;
}
 
int main(void)
{
    pthread_t th1, th2;
    
    pthread_create(&th1, NULL, worker, "X");
    pthread_create(&th2, NULL, worker, "Y");

    sleep(2);
    
    printf("====> Cancelling Thread Y with thread ID %lu!!\n", (long)th2);
    pthread_cancel(th2);
    usleep(100000);
    
    printf("====> Cancelling Thread X with thread ID %lu!!\n", (long)th1);
    pthread_cancel(th1);
    
    printf("Exiting from main program.\n");
    return 0;
}