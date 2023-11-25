#ifndef PERIODIC_SETTINGS_H
#define PERIODIC_SETTINGS_H

struct periodic_thread
{
	struct timespec r;
	int thread_id;
	int period;
	int offset;
	uint32_t wcet;
};

void current_time();

void timespec_add_us(struct timespec *t, uint64_t us);

int timespec_cmp(struct timespec *a, struct timespec *b);

void start_periodic_timer(struct periodic_thread *perthread);

void wait_next_activation(struct periodic_thread *t);

void log_message(const char *message, int *thread_id);

#endif
