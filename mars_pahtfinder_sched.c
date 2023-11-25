/**
 * @file mars_pahtfinder_sched.c
 * @brief Ejemplo de planificación de tareas con prioridades fijas
 * @details Este programa es un ejemplo de planificación de tareas con prioridades fijas.
 * Se crean 5 hilos (threads) con prioridades fijas y se les asignan tareas con periodos y
 * tiempos de ejecución fijos. Se utiliza la política de planificación FIFO (First In First Out)
 * para la asignación de los hilos a los procesadores.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>

#include "periodic_settings.h"
#include "task_definition.h"

#define NUM_TAREAS 5

// static pthread_mutex_t lock;

void *thread1(void *pt);
void *thread2(void *pt);
void *thread3(void *pt);
void *thread4(void *pt);
void *thread5(void *pt);

int main(int argc, char *argv[])
{
    pthread_t tareas[NUM_TAREAS];
    pthread_attr_t my_attr;

    // Inicializar recurso compartido con malloc
    struct SharedResource info_bus;
    init_shared_resource(&info_bus);

    struct sched_param sched_bus_priority, data_priority;
    struct sched_param control_priority, radio_priority, video_priority;

    // Inicializar prioridades de los hilos (threads)
    sched_bus_priority.sched_priority = 90;
    data_priority.sched_priority = 89;
    control_priority.sched_priority = 88;
    radio_priority.sched_priority = 87;
    video_priority.sched_priority = 86;

    // Inicializar atributos de planeación de los hilos (threads)
    pthread_attr_init(&my_attr);

    // Setear prioridades de los hilos (threads)
    int rc = pthread_attr_setschedpolicy(&my_attr, SCHED_RR);
    if (rc != 0)
    {
        perror("Error al setear la política de planificación");
        return -1;
    }

    struct PeriodicThread *sched_bus = malloc(sizeof(struct PeriodicThread));
    struct PeriodicThread *data = malloc(sizeof(struct PeriodicThread));
    struct PeriodicThread *control = malloc(sizeof(struct PeriodicThread));
    struct PeriodicThread *radio = malloc(sizeof(struct PeriodicThread));
    struct PeriodicThread *video = malloc(sizeof(struct PeriodicThread));

    sched_bus->thread_id = 1;
    data->thread_id = 2;
    data->shared_resource = &info_bus;
    control->thread_id = 3;
    control->shared_resource = &info_bus;
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

    // pthread_mutex_destroy(&lock);
    pthread_attr_destroy(&my_attr);

    pthread_exit(NULL);

    return 0;
}

void *thread1(void *pt)
{
    char message[256];
    struct timeval start, end;
    struct PeriodicThread *temp = (struct PeriodicThread *)pt;

    temp->period = 25000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;
    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        gettimeofday(&start, NULL);
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            snprintf(message, sizeof(message), "Deadline missed for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }
        else
        {
            snprintf(message, sizeof(message), "Deadline met for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }

        gettimeofday(&end, NULL);
        uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        sched_bus_task(&temp->thread_id, &temp->wcet, &elapsed);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
}

void *thread2(void *pt)
{
    char message[256];
    struct timeval start, end;
    struct PeriodicThread *temp = (struct PeriodicThread *)pt;

    temp->period = 25000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;
    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        gettimeofday(&start, NULL);
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        // Acceder al recurso compartido
        snprintf(message, sizeof(message), "Accessing shared resource from thread %d: now %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
        log_message(message, "log_file.log");

        pthread_mutex_lock(&temp->shared_resource->mutex);
        temp->shared_resource->resource++;
        pthread_mutex_unlock(&temp->shared_resource->mutex);

        snprintf(message, sizeof(message), "Resource value: %d", temp->shared_resource->resource);
        log_message(message, "log_file_resource.log");

        snprintf(message, sizeof(message), "Leaving shared resource from thread %d: now %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
        log_message(message, "log_file.log");

        if (timespec_cmp(&now, &next) > 0)
        {
            snprintf(message, sizeof(message), "Deadline missed for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }
        else
        {
            snprintf(message, sizeof(message), "Deadline met for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }

        gettimeofday(&end, NULL);
        uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        data_task(&temp->thread_id, &temp->wcet, &elapsed);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
}

void *thread3(void *pt)
{
    char message[256];
    struct timeval start, end;
    struct PeriodicThread *temp = (struct PeriodicThread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;
    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        gettimeofday(&start, NULL);
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        // Acceder al recurso compartido
        snprintf(message, sizeof(message), "Accessing shared resource from thread %d: now %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
        log_message(message, "log_file.log");

        pthread_mutex_lock(&temp->shared_resource->mutex);
        temp->shared_resource->resource++;
        pthread_mutex_unlock(&temp->shared_resource->mutex);

        snprintf(message, sizeof(message), "Resource value: %d", temp->shared_resource->resource);
        log_message(message, "log_file_resource.log");

        snprintf(message, sizeof(message), "Leaving shared resource from thread %d: now %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
        log_message(message, "log_file.log");

        if (timespec_cmp(&now, &next) > 0)
        {
            snprintf(message, sizeof(message), "Deadline missed for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }
        else
        {
            snprintf(message, sizeof(message), "Deadline met for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }

        gettimeofday(&end, NULL);
        uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        control_task(&temp->thread_id, &temp->wcet, &elapsed);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
}

void *thread4(void *pt)
{
    char message[256];
    struct timeval start, end;
    struct PeriodicThread *temp = (struct PeriodicThread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;
    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        gettimeofday(&start, NULL);
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            snprintf(message, sizeof(message), "Deadline missed for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }
        else
        {
            snprintf(message, sizeof(message), "Deadline met for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }

        gettimeofday(&end, NULL);
        uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        radio_task(&temp->thread_id, &temp->wcet, &elapsed);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
}

void *thread5(void *pt)
{
    char message[256];
    struct timeval start, end;
    struct PeriodicThread *temp = (struct PeriodicThread *)pt;

    temp->period = 50000;
    temp->offset = 0;
    temp->wcet = 5000;

    struct timespec next, now;
    clock_gettime(CLOCK_REALTIME, &next);
    while (1)
    {
        gettimeofday(&start, NULL);
        clock_gettime(CLOCK_REALTIME, &now);
        timespec_add_us(&next, temp->period);

        if (timespec_cmp(&now, &next) > 0)
        {
            snprintf(message, sizeof(message), "Deadline missed for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }
        else
        {
            snprintf(message, sizeof(message), "Deadline met for theread %d: now: %ld sec %ld nsec    next: %ld sec %ld nsec", temp->thread_id, now.tv_sec, now.tv_nsec, next.tv_sec, next.tv_nsec);
            log_message(message, "log_file.log");
        }

        gettimeofday(&end, NULL);
        uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        video_task(&temp->thread_id, &temp->wcet, &elapsed);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
}
