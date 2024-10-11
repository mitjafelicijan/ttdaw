#include <stdio.h>
#include <unistd.h>

#include "interface.h"
#include "mutex.h"

void *interface(void *arg) {
	InterfaceArgs* args = (InterfaceArgs*)arg;
	(void)args;

	while(1) {
		// Do the thread stuff here.
		sleep(1);
		fprintf(stdout, "hi from interface thread\n");
	}
}
