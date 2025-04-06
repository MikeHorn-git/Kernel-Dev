// https://stackoverflow.com/questions/10885685/jiffies-how-to-calculate-seconds-elapsed
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

unsigned long uptime_seconds;
SYSCALL_DEFINE1(uptime, unsigned long __user *, uptime_in_seconds)
{
	uptime_seconds = jiffies / HZ; // Convert jiffies to seconds

	// Ensure the user-space pointer is valid before copying
	if (uptime_in_seconds) {
		if (copy_to_user(uptime_in_seconds, &uptime_seconds,
				 sizeof(unsigned long))) {
			return -EFAULT; // Return error if copy_to_user fails
		}
	}
	return 0;
}
