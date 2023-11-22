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
}

void *data_task(int *id, int *wcet)
{
    usleep(*wcet);
}

void *control_task(int *id, int *wcet)
{
    usleep(*wcet);
}

void *radio_task(int *id, int *wcet)
{
    usleep(*wcet);
}

void *video_task(int *id, int *wcet)
{
    usleep(*wcet);
}
