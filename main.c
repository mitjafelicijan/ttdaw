#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>

#include "version.h"

void help(const char *argv0) {
	printf("Usage: %s [options]\n"
			"\nAvailable options:\n"
			"  -h,--help                  this help\n"
			"  -v,--version               show version\n"
			"  -l,--list                  list available devices\n"
			"  -p,--port=client:port      device port\n",
			argv0);
}

int main(int argc, char *argv[]) {
	const char short_options[] = "hvlp";
	const struct option long_options[] = {
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ "list", 0, NULL, 'l' },
		{ "port", 1, NULL, 'p' },
		{ 0 },
	};

	int c;
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
			case 'h':
				help(argv[0]);
				return 0;
			case 'v':
				fprintf(stdout, "ttdaw version %s\n", TTDAW_VERSION);
				fprintf(stdout, "Website: %s.\n", TTDAW_WEBSITE);
				fprintf(stdout, "%s\n", TTDAW_LICENSE);
				fprintf(stdout, "%s\n", TTDAW_WARRANTY);
				fprintf(stdout, "\n%s\n", TTDAW_AUTHOR);
				return 0;
			case 'l':
				fprintf(stderr, "List feature is NOT implemented yet.\n");
				return 0;
			case 'p':
				fprintf(stderr, "Port feature is NOT implemented yet.\n");
				return 0;
			default:
				fprintf(stdout, "No option provided\n");
				return 0;
		}
	}

	return 0;
}

