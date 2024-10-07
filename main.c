#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>

void help(const char *argv0) {
	printf("Usage: %s [options]\n"
			"\nAvailable options:\n"
			"  -h,--help                  this help\n"
			"  -v,--version               show version\n"
			"  -p,--port=client:port      device port\n",
			argv0);
}

int main(int argc, char *argv[]) {
	const char short_options[] = "hvlp";
	const struct option long_options[] = {
		{"help", 0, NULL, 'h'},
		{"version", 0, NULL, 'v'},
		{"list", 0, NULL, 'l'},
		{"port", 1, NULL, 'p'},
		{0},
	};

	int c;
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
			case 'h':
				help(argv[0]);
				return 0;
			case 'v':
				fprintf(stdout, "Version\n");
				return 0;
			case 'l':
				fprintf(stdout, "List\n");
				return 0;
			case 'p':
				fprintf(stdout, "Port\n");
				return 0;
			default:
				fprintf(stdout, "No option provided\n");
				return 0;
		}
	}

	return 0;
}

