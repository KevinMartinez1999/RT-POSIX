#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL
#define DEBUG_MODE

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void current_time()
{
    time_t sec;
    suseconds_t usec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    usec = tv.tv_usec;

    printf("Current time: %ld.%06ld\n", sec, usec);
}

void timespec_add_us(struct timespec *t, uint32_t us)
{
    us *= 1000;
    us += t->tv_nsec;
    while (us >= NSEC_PER_SEC)
    {
        us -= NSEC_PER_SEC;
        t->tv_sec++;
    }
    t->tv_nsec = us;
}

int timespec_cmp(struct timespec *a, struct timespec *b)
{
    if (a->tv_sec > b->tv_sec)
        return 1;
    else if (a->tv_sec < b->tv_sec)
        return -1;
    else
    {
        if (a->tv_nsec > b->tv_nsec)
            return 1;
        else if (a->tv_nsec < b->tv_nsec)
            return -1;
        else
            return 0;
    }
}

void start_periodic_timer(struct PeriodicThread *pt)
{
#ifdef DEBUG_MODE
    printf("El hilo %d tiene un periodo de %d us\n", pt->id, pt->period);
    printf("El hilo %d tiene un offset de %d us\n\n", pt->id, pt->offset);
#endif

    clock_gettime(CLOCK_REALTIME, &pt->r);
    timespec_add_us(&pt->r, pt->offset);
}

void wait_next_activation(struct PeriodicThread *pt)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &pt->r, NULL);
    timespec_add_us(&pt->r, pt->period);
}

void log_message(char *message, char *filename)
{
    pthread_mutex_lock(&mutex);

    FILE *fp = fopen(filename, "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s\n", message);
        fclose(fp);
    }

    pthread_mutex_unlock(&mutex);
}
