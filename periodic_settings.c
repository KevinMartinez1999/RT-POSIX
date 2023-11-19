#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL

void current_time()
{
    time_t sec;
    suseconds_t usec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    usec = tv.tv_usec;

    printf("El tiempo actual es: %ld.%06ld\n", sec, usec);
}

static inline void timespec_add_us(struct timespec *t, uint64_t d)
{
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC)
    {
        d -= NSEC_PER_SEC; // Por cada resta de NSEC_PER_SEC debe agregar un segundo al tv_sec.
        t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

void wait_next_activation(struct periodic_thread *t)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->r, NULL);
    timespec_add_us(&t->r, t->period);
}

void start_periodic_timer(struct periodic_thread *perthread)
{
    // printf("Este hilo tiene un periodo de %d us\n", perthread->period);
    printf("El Hilo %d se ejecutara cada %d us\n", perthread->thread_id, perthread->period);
    printf("El offset de este hilo es %d us\n\n", perthread->offset);

    clock_gettime(CLOCK_REALTIME, &perthread->r);      // Get current time
    timespec_add_us(&perthread->r, perthread->offset); // Add offset
}
