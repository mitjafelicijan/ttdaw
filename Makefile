tests: build-portmidi
	$(CC) -Wall example1.c minisdl_audio.c -lm -ldl -lpthread -o example1
	$(CC) -Wall example2.c minisdl_audio.c -lm -ldl -lpthread -o example2
	$(CC) -Wall example3.c -L./portmidi -lportmidi -o example3

run-example3: tests
	LD_LIBRARY_PATH=./portmidi ./example3

run-example4:
	$(CC) example4.c -lasound -lm -o example4 && ./example4

build-portmidi:
	cd portmidi && cmake . && make
