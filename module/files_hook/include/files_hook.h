#ifndef FILES_HOOK_H
#define FILES_HOOK_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

/* For each probe you need to allocate a kprobe structure */
extern struct kprobe kp;

/* Custom linux_dirent structure */
struct linux_dirent {
	unsigned long d_ino; // Inode number
	unsigned long d_off; // Offset to next dirent
	unsigned short d_reclen; // Length of this record
	char d_name[]; // Filename (null-terminated)
};

/* Function prototypes */
int pre_handler(struct kprobe *p, struct pt_regs *regs);
int hook_getdents(unsigned int fd, struct linux_dirent *dirp,
		  unsigned int count);

typedef asmlinkage int (*getdents_t)(unsigned int, struct linux_dirent *,
				     unsigned int);
extern getdents_t original_getdents;

#endif // FILES_HOOK_H
