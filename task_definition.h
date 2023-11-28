#ifndef TASK_DEFINITION_H
#define TASK_DEFINITION_H

void *sched_bus_task(uint32_t wcet);
void *data_task(uint32_t wcet);
void *control_task(uint32_t wcet);
void *radio_task(uint32_t wcet);
void *video_task(uint32_t wcet);

#endif
