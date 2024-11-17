#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
		return 1;
	}

	pid_t pid = atoi(argv[1]);
	char path[256];

	// Format the path for the /proc/[pid] directory
	snprintf(path, sizeof(path), "/proc/%d", pid);

	// Check if the directory exists
	if (access(path, F_OK) == -1) {
		printf("PID %d is hidden (does not exist in /proc)\n", pid);
	} else {
		printf("PID %d is visible (exists in /proc)\n", pid);
	}

	return 0;
}
