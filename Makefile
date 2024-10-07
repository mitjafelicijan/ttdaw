tests: build-portmidi
	$(CC) -Wall example1.c minisdl_audio.c -lm -ldl -lpthread -o example1
	$(CC) -Wall example2.c minisdl_audio.c -lm -ldl -lpthread -o example2
	$(CC) -Wall example3.c -L./portmidi -lportmidi -o example3

run-example3:
	LD_LIBRARY_PATH=./portmidi ./example3

build-portmidi:
	cd portmidi && cmake . && make
