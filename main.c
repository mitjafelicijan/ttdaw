#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <pthread.h>

#include "version.h"
#include "midi.h"
#include "synth.h"
#include "interface.h"
#include "mutex.h"

void help(const char *argv0) {
	printf("Usage: %s [options]\n"
			"\nAvailable options:\n"
			"  -n,--new                     creates a new song file\n"
			"  -l,--list                    list available devices\n"
			"  -c,--client=client:port      device client and port\n"
			"  -s,--soundfont=file.sf2      soundfont file\n"
			"  -p,--preset=1                soundfont preset\n"
			"  -h,--help                    this help\n"
			"  -v,--version                 show version\n",
			argv0);
}

int main(int argc, char *argv[]) {
	const char short_options[] = "n:lc:s:p:hv";
	const struct option long_options[] = {
		{ "new", 1, NULL, 'n' },
		{ "list", 0, NULL, 'l' },
		{ "client", 1, NULL, 'c' },
		{ "soundfont", 1, NULL, 's' },
		{ "preset", 0, NULL, 'p' },
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ 0 },
	};

	char *new_song_name = NULL;
	char *port_name = NULL;
	char *soundfont_file = NULL;
	int soundfont_preset = 0;

	int opt;
	while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (opt) {
			case 'n':
				new_song_name = optarg;
			case 'l':
				fprintf(stderr, "List feature is NOT implemented yet.\n");
				return 0;
			case 'c':
				port_name = optarg;
				break;
			case 's':
				soundfont_file = optarg;
				break;
			case 'p':
				soundfont_preset = atoi(optarg);
				break;
			case 'h':
				help(argv[0]);
				return 0;
			case 'v':
				fprintf(stdout, "ttdaw version %s\n", VERSION);
				fprintf(stdout, "Website: %s.\n", WEBSITE);
				fprintf(stdout, "%s\n", LICENSE);
				fprintf(stdout, "%s\n", WARRANTY);
				fprintf(stdout, "\n%s\n", AUTHOR);
				return 0;
			default:
				fprintf(stdout, "Missing options. Check help.\n");
				return 0;
		}
	}

	if (new_song_name != NULL) {
		fprintf(stdout, "Creating new song file with name %s\n", new_song_name);
		return 0;
	}

	if (port_name == NULL || soundfont_file == NULL || soundfont_preset < 0) {
		fprintf(stdout, "Missing options. Check help.\n");
		fprintf(stdout, "Port, soundfile and preset are required fields.\n");
		return 1;
	}

	/* fprintf(stdout, "> Device port:   %s\n", port_name); */
	/* fprintf(stdout, "> Soundfont:     %s\n", soundfont_file); */
	/* fprintf(stdout, "> SF preset:     %d\n", soundfont_preset); */
	/* fprintf(stdout, "> Song name:     %s\n", new_song_name); */

	// Create mutex.
	initialize_mutex();	

	// Create UI thread.
	pthread_t interface_thread;
	InterfaceArgs interface_args = {
		.soundfont_file = soundfont_file,
		.soundfont_preset = soundfont_preset,
	};

	if (pthread_create(&interface_thread, NULL, interface, (void*)&interface_args) != 0) {
		fprintf(stderr, "Error creating interface thread\n");
		return 1;
	}

	// Create synth thread.
	pthread_t synth_thread;
	SynthArgs synth_args = {
		.soundfont_file = soundfont_file,
		.soundfont_preset = soundfont_preset,
	};

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

	// Start threads.
	pthread_join(interface_thread, NULL);
	pthread_join(midi_thread, NULL);
	pthread_join(synth_thread, NULL);

	// Destroy mutex.
	destroy_mutex();

	fprintf(stdout, "Exiting...\n");
	return 0;
}

