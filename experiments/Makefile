CC      := cc
CFLAGS  := -Wall
LDFLAGS := -lm -ldl -lpthread -lasound

all: \
	soundfont_basic \
	soundfont_from_file \
	alsa_midi_controller \
	watch_file_changes

soundfont_basic: soundfont_basic.c
	$(CC) $(CFLAGS) soundfont_basic.c ../minisdl_audio.c $(LDFLAGS) -o soundfont_basic

soundfont_from_file: soundfont_from_file.c
	$(CC) $(CFLAGS) soundfont_from_file.c ../minisdl_audio.c $(LDFLAGS) -o soundfont_from_file

alsa_midi_controller: alsa_midi_controller.c
	 $(CC) $(CFLAGS) alsa_midi_controller.c $(LDFLAGS) -o alsa_midi_controller

watch_file_changes: watch_file_changes.c
	 $(CC) $(CFLAGS) watch_file_changes.c $(LDFLAGS) -o watch_file_changes

