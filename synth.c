#include <stdio.h>
#include <unistd.h>

#include "synth.h"
#include "minisdl_audio.h"

#define TSF_IMPLEMENTATION
#include "tsf.h"

static tsf* g_TinySoundFont;
static SDL_mutex* g_Mutex;

// Render the audio samples in float format.
static void AudioCallback(void* data, Uint8 *stream, int len) {
	int SampleCount = (len / (2 * sizeof(float))); // 2 output channels.
	SDL_LockMutex(g_Mutex); // Get exclusive lock.
	tsf_render_float(g_TinySoundFont, (float*)stream, SampleCount, 0);
	SDL_UnlockMutex(g_Mutex);
}

void *synth(void *arg) {
	SynthArgs* args = (SynthArgs*)arg;

	SDL_AudioSpec OutputAudioSpec;
	OutputAudioSpec.freq = 44100;
	OutputAudioSpec.format = AUDIO_F32;
	OutputAudioSpec.channels = 2;
	OutputAudioSpec.samples = 4096;
	OutputAudioSpec.callback = AudioCallback;

	// Initialize the audio system.
	if (SDL_AudioInit(TSF_NULL) < 0) {
		fprintf(stderr, "Could not initialize audio hardware or driver\n");
		exit(1);
	}

	// Load the SoundFont from a file.
	g_TinySoundFont = tsf_load_filename(args->soundfont_file);
	if (!g_TinySoundFont) {
		fprintf(stderr, "Could not load SoundFont\n");
		exit(1);
	}

	// Set the SoundFont rendering output mode.
	tsf_set_output(g_TinySoundFont, TSF_STEREO_INTERLEAVED, OutputAudioSpec.freq, 0);

	// Create the mutex.
	g_Mutex = SDL_CreateMutex();

	// Request the desired audio output format.
	if (SDL_OpenAudio(&OutputAudioSpec, TSF_NULL) < 0) {
		fprintf(stderr, "Could not open the audio hardware or the desired audio output format\n");
		exit(1);
	}

	SDL_PauseAudio(0);

	while (1) {
		sleep(1);

		SDL_LockMutex(g_Mutex);
		tsf_note_off(g_TinySoundFont, 1, 50);
		tsf_note_on(g_TinySoundFont, 1, 50, 1.0f);
		SDL_UnlockMutex(g_Mutex);
	}
}

