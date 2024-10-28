// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "sys_call_table",
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("<%s> p->addr = 0x%p, ip = %lx, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->ip, regs->flags);

	return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
	pr_info("<%s> p->addr = 0x%p, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->flags);
}

static int __init kallsyms_init(void)
{
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;

	register_kprobe(&kp);
	pr_info("sys_call_table: %px\n", kp.addr);
	return 0;
}

static void __exit kallsyms_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kallsyms_lookup exit successfully");

}

module_init(kallsyms_init)
module_exit(kallsyms_exit)
MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL");
