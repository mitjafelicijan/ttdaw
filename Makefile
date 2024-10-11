CC      := cc
CFLAGS  := -Wall -Wextra -Wshadow -Wunused -Wswitch-enum -Wpedantic -ggdb
LDFLAGS := -lm -ldl -lpthread -lasound
FILES   := main.c midi.c synth.c interface.c mutex.c minisdl_audio.c
PROG    := ttdaw

$(PROG): main.c
	$(CC) $(CFLAGS) -o $(PROG) $(FILES) $(LDFLAGS)

