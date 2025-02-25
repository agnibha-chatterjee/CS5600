#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
 
void *worker(void *data)
{
    char *name = (char*)data;
 
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
    sleep(1);
    printf("====> Cancelling Thread Y with thread ID %lu!!\n", (unsigned long)th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread X with thread ID %lu!!\n", (unsigned long)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}