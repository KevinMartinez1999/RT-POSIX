/**
 * @file task_definition.c
 * @brief Definición de tareas
 * @details Este archivo contiene las funciones para la definición de tareas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#include "task_definition.h"

#define MAX_TASKS 10

void *sched_bus_task(int *id, uint32_t *wcet, uint32_t *elapsed_time)
{
    usleep(*wcet - *elapsed_time);
}

void *data_task(int *id, uint32_t *wcet, uint32_t *elapsed_time)
{
    usleep(*wcet - *elapsed_time);
}

void *control_task(int *id, uint32_t *wcet, uint32_t *elapsed_time)
{
    usleep(*wcet - *elapsed_time);
}

void *radio_task(int *id, uint32_t *wcet, uint32_t *elapsed_time)
{
    usleep(*wcet - *elapsed_time);
}

void *video_task(int *id, uint32_t *wcet, uint32_t *elapsed_time)
{
    usleep(*wcet - *elapsed_time);
}
