#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#include "task_definition.h"

static pthread_mutex_t lock;

void *sched_bus_task(int *id, int *wcet)
{
    usleep(*wcet);
    
    pthread_mutex_lock(&lock);
    printf("SCHED_BUS: %d\n", *id);
    pthread_mutex_unlock(&lock);
}

void *data_task(int *id, int *wcet)
{
    usleep(*wcet);

    pthread_mutex_lock(&lock);
    printf("DATA: %d\n", *id);
    pthread_mutex_unlock(&lock);
}

void *control_task(int *id, int *wcet)
{
    usleep(*wcet);

    pthread_mutex_lock(&lock);
    printf("CONTROL: %d\n", *id);
    pthread_mutex_unlock(&lock);
}

void *radio_task(int *id, int *wcet)
{
    usleep(*wcet);

    pthread_mutex_lock(&lock);
    printf("RADIO: %d\n", *id);
    pthread_mutex_unlock(&lock);
}

void *video_task(int *id, int *wcet)
{
    usleep(*wcet);

    pthread_mutex_lock(&lock);
    printf("VIDEO: %d\n", *id);
    pthread_mutex_unlock(&lock);
}
