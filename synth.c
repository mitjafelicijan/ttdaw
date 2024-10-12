#include <stdio.h>
#include <unistd.h>

#include "synth.h"
#include "mutex.h"
#include "minisdl_audio.h"
#include "termbox2.h"

#define TSF_IMPLEMENTATION
#include "tsf.h"

static tsf* g_TinySoundFont;
static SDL_mutex* g_Mutex;

// Render the audio samples in float format.
static void AudioCallback(void* data, Uint8 *stream, int len) {
	int SampleCount = (len / (AUDIO_CHANNELS * sizeof(float))); // 2 output channels.
	SDL_LockMutex(g_Mutex); // Get exclusive lock.
	tsf_render_float(g_TinySoundFont, (float*)stream, SampleCount, 0);
	SDL_UnlockMutex(g_Mutex);
}

void *synth(void *arg) {
	SynthArgs* args = (SynthArgs*)arg;

	SDL_AudioSpec OutputAudioSpec;
	OutputAudioSpec.freq = AUDIO_FREQ;
	OutputAudioSpec.format = AUDIO_F32;
	OutputAudioSpec.channels = AUDIO_CHANNELS;
	OutputAudioSpec.samples = AUDIO_SAMPLES;
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
		pthread_mutex_lock(&mutex);
		while (shared_data.action == 0) {
			pthread_cond_wait(&cond_synth, &mutex);
		}

		SDL_LockMutex(g_Mutex);

		if (shared_data.state == 0) {
			tsf_note_off(g_TinySoundFont, args->soundfont_preset, shared_data.note);
		} else {
			float normalized_velocity = (float)shared_data.velocity / 127.0f;
			tsf_note_on(g_TinySoundFont, args->soundfont_preset, shared_data.note, normalized_velocity);
		}

		SDL_UnlockMutex(g_Mutex);

		/* printf("Consumed: note=%d, state=%d velocity:%d\n", shared_data.note, shared_data.state, shared_data.velocity); */

		// Reset state to indicate data has been consumed.
		shared_data.action = 0; 

		pthread_mutex_unlock(&mutex);
	}
}

