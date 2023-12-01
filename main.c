#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "task_definition.h"
#include "periodic_settings.h"

#define NUM_THREADS 5

pthread_mutexattr_t mymutexattr;
pthread_mutex_t mymutex1, mymutex2;

int resoruce = 0;

struct KeyValue
{
    uint32_t period;
    uint32_t offset;
    uint32_t wcet;
};

void *thread1(void *pt);
void *thread2(void *pt);
void *thread3(void *pt);
void *thread4(void *pt);
void *thread5(void *pt);

int main(void)
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t my_attr;
    void *(*thread_func[NUM_THREADS])(void *) = {thread1, thread2, thread3, thread4, thread5};
    int t, rc;

    struct sched_param param[NUM_THREADS] = {
        {.sched_priority = 90},
        {.sched_priority = 89},
        {.sched_priority = 88},
        {.sched_priority = 87},
        {.sched_priority = 86}};

    pthread_attr_init(&my_attr);
    pthread_mutexattr_init(&mymutexattr);
    pthread_mutexattr_setprotocol(&mymutexattr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mymutex1, &mymutexattr);
    pthread_mutex_init(&mymutex2, &mymutexattr);

    struct KeyValue kv[NUM_THREADS] = {
        {25000, 1000000, 5000},
        {25000, 1000000, 5000},
        {50000, 1000000, 5000},
        {50000, 1000000, 5000},
        {50000, 1000000, 5000}};

    struct PeriodicThread *pt;

    rc = pthread_attr_setschedpolicy(&my_attr, SCHED_RR);
    if (rc)
    {
        printf("ERROR; return code from pthread_attr_setschedpolicy() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_attr_setinheritsched(&my_attr, PTHREAD_EXPLICIT_SCHED);
    if (rc)
    {
        printf("ERROR; return code from pthread_attr_setinheritsched() is %d\n", rc);
        exit(-1);
    }

    for (t = 0; t < NUM_THREADS; t++)
    {
        pt = malloc(sizeof(struct PeriodicThread));
        pt->id = t + 1;
        pt->period = kv[t].period;
        pt->offset = kv[t].offset;
        pt->wcet = kv[t].wcet;

        rc = pthread_attr_setschedparam(&my_attr, &param[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_attr_setschedparam() is %d\n", rc);
            exit(-1);
        }

        rc = pthread_create(&threads[t], &my_attr, thread_func[t], (void *)pt);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_join(threads[t], NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&my_attr);
    pthread_mutexattr_destroy(&mymutexattr);
    pthread_mutex_destroy(&mymutex1);
    pthread_mutex_destroy(&mymutex2);
    pthread_exit(NULL);

    return 0;
}

void *thread1(void *pt)
{
    char message[256];
    struct timespec now;
    struct PeriodicThread *p = (struct PeriodicThread *)pt;

    start_periodic_timer(p);

    while (1)
    {
        wait_next_activation(p);
        task_definition(p->wcet);

        clock_gettime(CLOCK_REALTIME, &now);
        if (timespec_cmp(&now, &p->r) > 0)
        {
            sprintf(message, "Thread %d: Deadline missed: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
        else
        {
            sprintf(message, "Thread %d: Deadline met: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
    }
}

void *thread2(void *pt)
{
    char message[256];
    struct timespec now;
    struct PeriodicThread *p = (struct PeriodicThread *)pt;

    start_periodic_timer(p);

    while (1)
    {
        wait_next_activation(p);
        task_definition(p->wcet);

        clock_gettime(CLOCK_REALTIME, &now);
        snprintf(message, sizeof(message), "Thread %d: Accessing resource: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
        log_message(message, "logfile.log");

        pthread_mutex_lock(&mymutex1);
        int i;
        while (i < 100000)
        {
            i++;
        }
        resoruce++;
        pthread_mutex_unlock(&mymutex1);

        clock_gettime(CLOCK_REALTIME, &now);
        snprintf(message, sizeof(message), "Thread %d: Releasing resource: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
        log_message(message, "logfile.log");

        clock_gettime(CLOCK_REALTIME, &now);
        if (timespec_cmp(&now, &p->r) > 0)
        {
            sprintf(message, "Thread %d: Deadline missed: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
        else
        {
            sprintf(message, "Thread %d: Deadline met: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
    }
}

void *thread3(void *pt)
{
    char message[256];
    struct timespec now;
    struct PeriodicThread *p = (struct PeriodicThread *)pt;

    start_periodic_timer(p);

    while (1)
    {
        wait_next_activation(p);
        task_definition(p->wcet);

        clock_gettime(CLOCK_REALTIME, &now);
        snprintf(message, sizeof(message), "Thread %d: Accessing resource: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
        log_message(message, "logfile.log");

        pthread_mutex_lock(&mymutex2);
        int i;
        while (i < 100000)
        {
            i++;
        }
        resoruce++;
        pthread_mutex_unlock(&mymutex2);

        clock_gettime(CLOCK_REALTIME, &now);
        snprintf(message, sizeof(message), "Thread %d: Releasing resource: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
        log_message(message, "logfile.log");

        clock_gettime(CLOCK_REALTIME, &now);
        if (timespec_cmp(&now, &p->r) > 0)
        {
            sprintf(message, "Thread %d: Deadline missed: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
        else
        {
            sprintf(message, "Thread %d: Deadline met: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
    }
}

void *thread4(void *pt)
{
    char message[256];
    struct timespec now;
    struct PeriodicThread *p = (struct PeriodicThread *)pt;

    start_periodic_timer(p);

    while (1)
    {
        wait_next_activation(p);
        task_definition(p->wcet);

        clock_gettime(CLOCK_REALTIME, &now);
        if (timespec_cmp(&now, &p->r) > 0)
        {
            sprintf(message, "Thread %d: Deadline missed: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
        else
        {
            sprintf(message, "Thread %d: Deadline met: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
    }
}

void *thread5(void *pt)
{
    char message[256];
    struct timespec now;
    struct PeriodicThread *p = (struct PeriodicThread *)pt;

    start_periodic_timer(p);

    while (1)
    {
        wait_next_activation(p);
        task_definition(p->wcet);

        clock_gettime(CLOCK_REALTIME, &now);
        if (timespec_cmp(&now, &p->r) > 0)
        {
            sprintf(message, "Thread %d: Deadline missed: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
        else
        {
            sprintf(message, "Thread %d: Deadline met: Now: %ld sec %ld nsec, Deadline: %ld sec %ld nsec", p->id, now.tv_sec, now.tv_nsec, p->r.tv_sec, p->r.tv_nsec);
            log_message(message, "logfile.log");
        }
    }
}
