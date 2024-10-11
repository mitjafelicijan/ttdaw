#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

typedef struct {
	int note;
	int state;
	int velocity;
	int action;
} SharedData;

extern SharedData shared_data;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond_midi;
extern pthread_cond_t cond_synth;

void initialize_mutex();
void destroy_mutex();

#endif // MUTEX_H_

