#ifndef PERIODIC_SETTINGS_H
#define PERIODIC_SETTINGS_H

struct SharedResource
{
	uint32_t resource;
	pthread_mutex_t mutex;
};

struct PeriodicThread
{
	struct timespec r;
	int thread_id;
	int period;
	int offset;
	uint32_t wcet;
	struct SharedResource *shared_resource;
};

void current_time();

void timespec_add_us(struct timespec *t, uint64_t us);

int timespec_cmp(struct timespec *a, struct timespec *b);

void start_periodic_timer(struct PeriodicThread *perthread);

void wait_next_activation(struct PeriodicThread *t);

void log_message(const char *message, const char *filename);

void init_shared_resource(struct SharedResource *shared_resource);

#endif
