#include <alsa/asoundlib.h>

#ifndef MIDI_H_
#define MIDI_H_

#define CLIENT_NAME "ttdaw"
#define MAX_MIDI_PORTS 1

typedef struct {
	char *port_name;
} MidiArgs;

void *midi(void *arg);

#endif //  MIDI_H_

