#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

typedef struct {
	int note;
	int state;
	int velocity;
	int preset;
	int action;
} SharedData;

extern SharedData shared_data;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond_midi;
extern pthread_cond_t cond_synth;

void initialize_mutex();
void destroy_mutex();

#endif // MUTEX_H

