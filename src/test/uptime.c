#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_uptime 467 // depend of your syscall

int main()
{
	unsigned long uptime_in_seconds;
	long result;
	result = syscall(__NR_uptime, &uptime_in_seconds);
	if (result == 0)
		printf("Uptime: %lu seconds\n", uptime_in_seconds);
	else
		perror("Syscall failed");
	return 0;
}
