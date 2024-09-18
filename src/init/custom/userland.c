#include <unistd.h>
#include <sys/syscall.h>

#define MY_SYSCALL_NUMBER 466 // depend of your syscall

int main()
{
    syscall(MY_SYSCALL_NUMBER);
    return 0;
}
