#ifndef SYNTH_H_
#define SYNTH_H_

typedef struct {
	char *soundfont_file;
} SynthArgs;

void *synth(void *arg);

#endif //  SYNTH_H_

