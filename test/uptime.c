#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_uptime 467 // depend of your syscall

int main()
{
    long result;
    result = syscall(__NR_uptime);

    if (result == 0)
        printf("Syscall success\n");
    else
        perror("Syscall failed\n");

    return 0;
}
