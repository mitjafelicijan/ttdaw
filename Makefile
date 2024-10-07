CC      := cc
CFLAGS  := -Wall -Wextra -Wshadow -Wunused -Wswitch-enum -Wpedantic -Wundef
LDFLAGS := -lm -ldl -lpthread -lasound

ttdaw: main.c
	$(CC) $(CFLAGS) -o ttdaw main.c minisdl_audio.c $(LDFLAGS)

