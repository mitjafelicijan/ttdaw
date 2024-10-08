#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int main(void) {
	struct stat file_stat;
	time_t last_mod_time;
	const char* file = "watch_file_changes.txt";

	stat(file, &file_stat);
	last_mod_time = file_stat.st_mtime;

	while (1) {
		sleep(1);
		stat(file, &file_stat);
		if (file_stat.st_mtime != last_mod_time) {
			last_mod_time = file_stat.st_mtime;

			struct tm* time_info = localtime(&last_mod_time);
			char formatted_time[9];
			strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", time_info);

			printf("File %s changed at %s\n", file, formatted_time);
		}
	}
	printf("hi\n");
	return 0;
}

