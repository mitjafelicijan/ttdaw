#ifndef INTERFACE_H
#define INTERFACE_H

typedef struct {
	char *soundfont_file;
	int soundfont_preset;
} InterfaceArgs;

void *interface(void *arg);

#endif // INTERFACE_H

