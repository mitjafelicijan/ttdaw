#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portmidi/pm_common/portmidi.h"

#define NUM_INPUTS 16

int main() {
	PmError err;
	PmStream *stream;
	PmEvent events[128];
	int i, num_events;

	// Initialize PortMIDI
	err = Pm_Initialize();
	if (err != pmNoError) {
		fprintf(stderr, "Error initializing PortMIDI: %s\n", Pm_GetErrorText(err));
		exit(1);
	}

	/* // Open a MIDI input device */
	stream = (PortMidiStream *)Pm_OpenInput(&err, NULL, NULL, NUM_INPUTS, NULL, 0);
	if (err != pmNoError) {
	    fprintf(stderr, "Error opening MIDI input device: %s\n", Pm_GetErrorText(err));
	    Pm_Terminate();
	    exit(1);
	}

	/* // Read MIDI messages from the input device */
	/* while (1) { */
	/*     num_events = Pm_Read(stream, events, 128); */
	/*     if (num_events > 0) { */
	/*         for (i = 0; i < num_events; i++) { */
	/*             if (events[i].message & 0xff00) { */
	/*                 // This is a status message (note on, note off, etc.) */
	/* printf("Message: 0x%02x\n", events[i].message); */
	/*                 /1* printf("Status: 0x%02x, Data 1: 0x%02x, Data 2: 0x%02x\n", *1/ */
	/*                 /1*        events[i].message & 0xf0, events[i].message[0], events[i].message[1]); *1/ */
	/*             } */
	/*         } */
	/*     } */
	/* } */

	/* // Close the MIDI input device */
	Pm_Close(stream);

	// Terminate PortMIDI
	Pm_Terminate();

	return 0;
}

