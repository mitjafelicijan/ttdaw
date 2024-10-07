#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#define CLIENT_NAME "ttdaw"
#define MAX_MIDI_PORTS 4

static snd_seq_t *seq_handle;
static snd_seq_addr_t *ports;
static int rate = 44100;

int main(void) {
	fprintf(stdout, "Example: Reading MIDI input\n");

	snd_seq_t *seq_handle;

	if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
		fprintf(stderr, "Error opening ALSA sequencer.\n");
		exit(1);
	}

	if (snd_seq_set_client_name(seq_handle, CLIENT_NAME) < 0) {
		fprintf(stderr, "Could not set up client name.\n");
		exit(1);
	}

	if (snd_seq_create_simple_port(seq_handle, CLIENT_NAME,
				SND_SEQ_PORT_CAP_WRITE |
				SND_SEQ_PORT_CAP_SUBS_WRITE,
				SND_SEQ_PORT_TYPE_MIDI_GENERIC |
				SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
		fprintf(stderr, "Error creating sequencer port.\n");
		exit(1);
	}

	char *port_name = "28:0";
	if (snd_seq_parse_address(seq_handle, &ports[0], port_name) < 0) {
		fprintf(stderr, "Invalid port %s.\n", port_name);
		exit(1);
	}

	// Connecting ports.
	for (int i = 0; i < MAX_MIDI_PORTS; ++i) {
		int err = snd_seq_connect_from(seq_handle, 0, ports[i].client, ports[i].port);
		if (err < 0) {
			fprintf(stderr, "Cannot connect from port %d:%d - %s", ports[i].client, ports[i].port, snd_strerror(err));
			exit(1);
		}
	}

	snd_seq_close(seq_handle);
	return 0;
}

