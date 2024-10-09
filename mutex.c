#include <pthread.h>
#include "mutex.h"

SharedData shared_data;

pthread_mutex_t mutex;
pthread_cond_t cond_midi;
pthread_cond_t cond_synth;

void initialize_mutex() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_synth, NULL);
}

void destroy_mutex() {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_synth);
}

