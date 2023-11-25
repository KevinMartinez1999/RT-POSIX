#ifndef TASK_DEFINITION_H
#define TASK_DEFINITION_H

/**
 * TAREAS:
 * 1. SCHED_BUS
 * 2. DATA
 * 3. CONTROL
 * 4. RADIO
 * 5. VIDEO
 */

void *sched_bus_task(int *id, uint32_t *wcet, uint32_t *elapsed_time);
void *data_task(int *id, uint32_t *wcet, uint32_t *elapsed_time);
void *control_task(int *id, uint32_t *wcet, uint32_t *elapsed_time);
void *radio_task(int *id, uint32_t *wcet, uint32_t *elapsed_time);
void *video_task(int *id, uint32_t *wcet, uint32_t *elapsed_time);

#endif
