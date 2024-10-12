#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#include "midi.h"
#include "mutex.h"
#include "termbox2.h"

static snd_seq_t *seq_handle;
static snd_seq_addr_t *ports;

void *midi(void *arg) {
	MidiArgs* args = (MidiArgs*)arg;

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
	ports = realloc(ports, MAX_MIDI_PORTS * sizeof(snd_seq_addr_t));
	if (snd_seq_parse_address(seq_handle, &ports[0], args->port_name) < 0) {
		fprintf(stderr, "Invalid port %s.\n", args->port_name);
		exit(1);
	}

	// Listing assigned ports.
	/* fprintf(stdout, "Ports:\n"); */
	/* for (int j = 0; j < MAX_MIDI_PORTS; j++) { */
	/* 	fprintf(stdout, " client: %d, port: %d\n", ports[j].client, ports[j].port); */
	/* } */

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
				pthread_mutex_lock(&mutex);
				switch (ev->type) {
					case SND_SEQ_EVENT_NOTEON:
						shared_data.note = ev->data.note.note;
						shared_data.state = 1;
						shared_data.velocity = ev->data.note.velocity;
						tb_printf(0, 3, TB_CYAN, 0, "Note: %-3d", ev->data.note.note);
						tb_printf(0, 4, TB_CYAN, 0, "Velocity: %-3d", ev->data.note.velocity);
						tb_present();
						/* printf("%3d:%-3dNote on  %2d, note %d, velocity: %3d\n", */
						/* 		ev->source.client, ev->source.port, */
						/* 		ev->data.note.channel, */
						/* 		ev->data.note.note, */
						/* 		ev->data.note.velocity); */
						break;

					case SND_SEQ_EVENT_NOTEOFF:
						shared_data.note = ev->data.note.note;
						shared_data.state = 0;
						shared_data.velocity = 0;
						/* printf("%3d:%-3dNote off\t%2d, note %d\n", */
						/* 		ev->source.client, ev->source.port, */
						/* 		ev->data.note.channel, */
						/* 		ev->data.note.note); */
						break;


				}

				shared_data.action = 1;

				pthread_cond_signal(&cond_synth);
				pthread_mutex_unlock(&mutex);
			}
		}

		fflush(stdout);
	}
}

