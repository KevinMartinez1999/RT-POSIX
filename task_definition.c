#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "task_definition.h"

void task_definition(uint32_t wcet)
{
    usleep(wcet);
}
