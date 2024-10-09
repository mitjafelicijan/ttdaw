#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <pthread.h>

#include "version.h"
#include "midi.h"
#include "synth.h"

void help(const char *argv0) {
	printf("Usage: %s [options]\n"
			"\nAvailable options:\n"
			"  -l,--list                    list available devices\n"
			"  -p,--port=client:port        device port\n"
			"  -s,--soundfont=file.sf2      soundfont file\n"
			"  -h,--help                    this help\n"
			"  -v,--version                 show version\n",
			argv0);
}

int main(int argc, char *argv[]) {
	const char short_options[] = "lp:s:hv:";
	const struct option long_options[] = {
		{ "list", 0, NULL, 'l' },
		{ "port", 1, NULL, 'p' },
		{ "soundfont", 1, NULL, 's' },
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ 0 },
	};

	char *port_name = NULL;
	char *soundfont_file = NULL;

	int opt;
	while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (opt) {
			case 'l':
				fprintf(stderr, "List feature is NOT implemented yet.\n");
				return 0;
			case 'p':
				port_name = optarg;
				break;
			case 's':
				soundfont_file = optarg;
				break;
			case 'h':
				help(argv[0]);
				return 0;
			case 'v':
				fprintf(stdout, "ttdaw version %s\n", TTDAW_VERSION);
				fprintf(stdout, "Website: %s.\n", TTDAW_WEBSITE);
				fprintf(stdout, "%s\n", TTDAW_LICENSE);
				fprintf(stdout, "%s\n", TTDAW_WARRANTY);
				fprintf(stdout, "\n%s\n", TTDAW_AUTHOR);
				return 0;
			default:
				fprintf(stdout, "Missing options. Check help.\n");
				return 0;
		}
	}

	if (port_name == NULL || soundfont_file == NULL) {
		fprintf(stdout, "Missing options. Check help.\n\n");
		fprintf(stdout, "Port and soundfile are required fields.\n\n");
		return 1;
	}

	fprintf(stdout, "> Device port:   %s\n", port_name);
	fprintf(stdout, "> Soundfont:     %s\n", soundfont_file);

	// Create synth thread.
	pthread_t synth_thread;
	SynthArgs synth_args = { soundfont_file };

	if (pthread_create(&synth_thread, NULL, synth, (void*)&synth_args) != 0) {
		fprintf(stderr, "Error creating synth thread\n");
		return 1;
	}

	// Create MIDI thread.
	pthread_t midi_thread;
	MidiArgs midi_args = { port_name };

	if (pthread_create(&midi_thread, NULL, midi, (void*)&midi_args) != 0) {
		fprintf(stderr, "Error creating midi thread\n");
		return 1;
	}

	pthread_join(midi_thread, NULL);
	pthread_join(synth_thread, NULL);

	fprintf(stdout, "Exiting...\n");
	return 0;
}

