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
    pthread_attr_t my_attr;

    struct sched_param sched_bus_priority, data_priority, control_priority, radio_priority, video_priority;
    sched_bus_priority.sched_priority = 90;
    data_priority.sched_priority = 89;
    control_priority.sched_priority = 88;
    radio_priority.sched_priority = 87;
    video_priority.sched_priority = 86;

    // Inicializar atributos de planeación de los hilos (threads)
    pthread_attr_init(&my_attr);

    // Setear prioridades de los hilos (threads)
    int rc = pthread_attr_setschedpolicy(&my_attr, SCHED_FIFO);
    if (rc != 0)
    {
        perror("Error al setear la política de planificación");
        return -1;
    }

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

    pthread_attr_setinheritsched(&my_attr, PTHREAD_EXPLICIT_SCHED);

    pthread_attr_setschedparam(&my_attr, &sched_bus_priority);
    int rc1 = pthread_create(&tareas[0], &my_attr, thread1, (void *)sched_bus);

    pthread_attr_setschedparam(&my_attr, &data_priority);
    int rc2 = pthread_create(&tareas[1], &my_attr, thread2, (void *)data);

    pthread_attr_setschedparam(&my_attr, &control_priority);
    int rc3 = pthread_create(&tareas[2], &my_attr, thread3, (void *)control);

    pthread_attr_setschedparam(&my_attr, &radio_priority);
    int rc4 = pthread_create(&tareas[3], &my_attr, thread4, (void *)radio);

    pthread_attr_setschedparam(&my_attr, &video_priority);
    int rc5 = pthread_create(&tareas[4], &my_attr, thread5, (void *)video);

    if (rc1 || rc2 || rc3 || rc4 || rc5)
    {
        printf("ERROR; There was a problem creating a thread.\n");
        exit(-1);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_TAREAS; i++)
    {
        pthread_join(tareas[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_attr_destroy(&my_attr);

    pthread_exit(NULL);

    return 0;
}

void *thread1(void *pt)
{
    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 25000;   // 25 ms
    temp->offset = 0;       // 100 ms
    temp->wcet = 5000;      // 5 ms

    struct timespec next, now;

    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            fprintf(stderr, "Deadline missed for theread %d\n", temp->thread_id);
            fprintf(stderr, "now: %ld sec %ld nsec      next: %ld sec %ld nsec\n", now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            exit(-1);
        }

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);

        sched_bus_task(&temp->thread_id, &temp->wcet);
    }

    return NULL;
}

void *thread2(void *pt)
{
    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 25000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;

    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            fprintf(stderr, "Deadline missed for theread %d\n", temp->thread_id);
            fprintf(stderr, "now: %ld sec %ld nsec      next: %ld sec %ld nsec\n", now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            exit(-1);
        }

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);

        data_task(&temp->thread_id, &temp->wcet);
    }

    return NULL;
}

void *thread3(void *pt)
{
    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;

    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            fprintf(stderr, "Deadline missed for theread %d\n", temp->thread_id);
            fprintf(stderr, "now: %ld sec %ld nsec      next: %ld sec %ld nsec\n", now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            exit(-1);
        }

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);

        control_task(&temp->thread_id, &temp->wcet);
    }

    return NULL;
}

void *thread4(void *pt)
{
    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;

    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            fprintf(stderr, "Deadline missed for theread %d\n", temp->thread_id);
            fprintf(stderr, "now: %ld sec %ld nsec      next: %ld sec %ld nsec\n", now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            exit(-1);
        }

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);

        radio_task(&temp->thread_id, &temp->wcet);
    }

    return NULL;
}

void *thread5(void *pt)
{
    struct periodic_thread *temp = (struct periodic_thread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;

    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            fprintf(stderr, "Deadline missed for theread %d\n", temp->thread_id);
            fprintf(stderr, "now: %ld sec %ld nsec      next: %ld sec %ld nsec\n", now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            exit(-1);
        }

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);

        video_task(&temp->thread_id, &temp->wcet);
    }

    return NULL;
}
