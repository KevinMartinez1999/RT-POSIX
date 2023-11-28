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

void *sched_bus_task(uint32_t wcet)
{
    usleep(wcet);
}

void *data_task(uint32_t wcet)
{
    usleep(wcet);
}

void *control_task(uint32_t wcet)
{
    usleep(wcet);
}

void *radio_task(uint32_t wcet)
{
    usleep(wcet);
}

void *video_task(uint32_t wcet)
{
    usleep(wcet);
}
