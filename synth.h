#ifndef SYNTH_H_
#define SYNTH_H_

#define AUDIO_FREQ 44100
#define AUDIO_SAMPLES 64
#define AUDIO_CHANNELS 2

typedef struct {
	char *soundfont_file;
	int soundfont_preset;
} SynthArgs;

void *synth(void *arg);

#endif //  SYNTH_H_

