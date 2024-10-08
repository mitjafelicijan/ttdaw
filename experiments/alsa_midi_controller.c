#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <signal.h>
#include <alsa/asoundlib.h>

#define CLIENT_NAME "ttdaw"
#define MAX_MIDI_PORTS 1

static snd_seq_t *seq_handle;
static snd_seq_addr_t *ports;
static int stop = 0;

static void sighandler(int sig ATTRIBUTE_UNUSED) {
	stop = 1;
}

int main(void) {
	fprintf(stdout, "Example: Reading MIDI input\n");

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

	// Connecting ports.
	char *port_name = "28:0";
	ports = realloc(ports, MAX_MIDI_PORTS * sizeof(snd_seq_addr_t));
	if (snd_seq_parse_address(seq_handle, &ports[0], port_name) < 0) {
		fprintf(stderr, "Invalid port %s.\n", port_name);
		exit(1);
	}

	// Listing assigned ports.
	fprintf(stdout, "Ports:\n");
	for (int j = 0; j < MAX_MIDI_PORTS; j++) {
		fprintf(stdout, " client: %d, port: %d\n", ports[j].client, ports[j].port);
	}

	// Connecting ports.
	for (int i = 0; i < MAX_MIDI_PORTS; ++i) {
		int err = snd_seq_connect_from(seq_handle, 0, ports[i].client, ports[i].port);
		if (err < 0) {
			fprintf(stderr, "Cannot connect from port %d:%d - %s", ports[i].client, ports[i].port, snd_strerror(err));
			exit(1);
		}
	}

	if (snd_seq_nonblock(seq_handle, 1) < 0) {
		fprintf(stderr, "Set nonblock mode failed.");
		exit(1);
	}

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	// Reading MIDI device.
	struct pollfd *pfds;
	size_t npfds;

	npfds = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
	pfds = alloca(sizeof(*pfds) * npfds);

	for (;;) {
		snd_seq_poll_descriptors(seq_handle, pfds, npfds, POLLIN);
		if (poll(pfds, npfds, -1) < 0) {
			break;
		}

		for (;;) {
			snd_seq_event_t *ev;

			if (snd_seq_event_input(seq_handle, &ev) < 0) {
				break;
			}

			if (ev) {
				switch (ev->type) {
					case SND_SEQ_EVENT_NOTEON:
						printf("%3d:%-3dNote on  %2d, note %d, velocity: %3d\n",
								ev->source.client, ev->source.port,
								ev->data.note.channel,
								ev->data.note.note,
								ev->data.note.velocity);
						break;

					case SND_SEQ_EVENT_NOTEOFF:
						printf("%3d:%-3dNote off\t%2d, note %d\n",
								ev->source.client, ev->source.port,
								ev->data.note.channel,
								ev->data.note.note);
						break;


				} 
			}
		}

		fflush(stdout);

		if (stop) {
			break;
		}
	}

	fprintf(stdout, "Finishing up\n");
	snd_seq_close(seq_handle);
	return 0;
}

