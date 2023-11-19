#ifndef PERIODIC_SETTINGS_H
#define PERIODIC_SETTINGS_H

struct periodic_thread
{
	struct timespec r;
	int thread_id;
	int period;
	int offset;
	int wcet;
	int priority;
};

void current_time();

void start_periodic_timer(struct periodic_thread *perthread);

void wait_next_activation(struct periodic_thread *t);

#endif
