#include <linux/kernel.h>
#include <linux/syscalls.h>

int hello(void);
int hello(void)
{
	printk(KERN_INFO "Hello World USER\n");
	return 0;
}

SYSCALL_DEFINE0(hello)
{
	printk(KERN_INFO "Hello World KERNEL\n");
	return 0;
}
