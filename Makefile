CC      := cc
CFLAGS  := -Wall -Wextra -Wshadow -Wunused -Wswitch-enum -Wpedantic -Wundef
LDFLAGS := -lm -ldl -lpthread -lasound
CFILES  := main.c minisdl_audio.c
PROG    := ttdaw

$(PROG): main.c
	$(CC) $(CFLAGS) -o $(PROG) $(CFILES) $(LDFLAGS)

