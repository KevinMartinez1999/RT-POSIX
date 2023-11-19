#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

#include "periodic_settings.h"
#include "task_definition.h"

#define NUM_TAREAS 5

static pthread_mutex_t lock;

void *thread1(void *pt);
void *thread2(void *pt);
void *thread3(void *pt);
void *thread4(void *pt);
void *thread5(void *pt);

int main(int argc, char *argv[])
{
    pthread_t tareas[NUM_TAREAS];

    struct periodic_thread *sched_bus = malloc(sizeof(struct periodic_thread));
    struct periodic_thread *data = malloc(sizeof(struct periodic_thread));
    struct periodic_thread *control = malloc(sizeof(struct periodic_thread));
    struct periodic_thread *radio = malloc(sizeof(struct periodic_thread));
    struct periodic_thread *video = malloc(sizeof(struct periodic_thread));

    sched_bus->thread_id = 1;
    data->thread_id = 2;
    control->thread_id = 3;
    radio->thread_id = 4;
    video->thread_id = 5;

    int rc1 = pthread_create(&tareas[0], NULL, thread1, (void *)sched_bus);
    int rc2 = pthread_create(&tareas[1], NULL, thread2, (void *)data);
    int rc3 = pthread_create(&tareas[2], NULL, thread3, (void *)control);
    int rc4 = pthread_create(&tareas[3], NULL, thread4, (void *)radio);
    int rc5 = pthread_create(&tareas[4], NULL, thread5, (void *)video);

    if (rc1 || rc2 || rc3 || rc4 || rc5)
    {
        printf("ERROR; There was a problem creating a thread.\n");
        exit(-1);
    }

    pthread_mutex_destroy(&lock);
	pthread_exit(NULL);

    return 0;
}

void *thread1(void *pt)
{
    pthread_mutex_lock(&lock);

    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 25000;   // 25 ms
    temp->offset = 100000;  // 100 ms
    temp->wcet = 5000;      // 5 ms
    temp->priority = 1;

    start_periodic_timer(temp);

    pthread_mutex_unlock(&lock);

    while(1)
    {
        wait_next_activation(temp);
        sched_bus_task(&temp->thread_id, &temp->wcet);
    }

    pthread_exit(NULL);
}

void *thread2(void *pt)
{
    pthread_mutex_lock(&lock);

    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 25000;   // 25 ms
    temp->offset = 100000;  // 100 ms
    temp->wcet = 5000;      // 5 ms
    temp->priority = 1;

    start_periodic_timer(temp);

    pthread_mutex_unlock(&lock);

    while(1)
    {
        wait_next_activation(temp);
        data_task(&temp->thread_id, &temp->wcet);
    }

    pthread_exit(NULL);

    return NULL;
}

void *thread3(void *pt)
{
    pthread_mutex_lock(&lock);

    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;   // 50 ms
    temp->offset = 100000;  // 100 ms
    temp->wcet = 5000;      // 5 ms
    temp->priority = 1;

    start_periodic_timer(temp);

    pthread_mutex_unlock(&lock);

    while(1)
    {
        wait_next_activation(temp);
        control_task(&temp->thread_id, &temp->wcet);
    }

    pthread_exit(NULL);

    return NULL;
}

void *thread4(void *pt)
{
    pthread_mutex_lock(&lock);

    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;   // 50 ms
    temp->offset = 100000;  // 100 ms
    temp->wcet = 5000;      // 5 ms
    temp->priority = 1;

    start_periodic_timer(temp);

    pthread_mutex_unlock(&lock);

    while(1)
    {
        wait_next_activation(temp);
        radio_task(&temp->thread_id, &temp->wcet);
    }

    pthread_exit(NULL);

    return NULL;
}

void *thread5(void *pt)
{
    pthread_mutex_lock(&lock);

    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;   // 50 ms
    temp->offset = 100000;  // 100 ms
    temp->wcet = 5000;      // 5 ms
    temp->priority = 1;

    start_periodic_timer(temp);

    pthread_mutex_unlock(&lock);

    while(1)
    {
        wait_next_activation(temp);
        video_task(&temp->thread_id, &temp->wcet);
    }

    pthread_exit(NULL);

    return NULL;
}
