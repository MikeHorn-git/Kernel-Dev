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
	.symbol_name = "sys_call_table",
};

static int __init kallsyms_lookup_init(void)
{
	register_kprobe(&kp);
	pr_info("sys_call_table: %px\n", kp.addr);
	return 0;
}

static void __exit kallsyms_lookup_exit(void)
{
	pr_info("Unregistering kprobe for sys_call_table\n");
	unregister_kprobe(&kp);
	pr_info("kallsyms_lookup exit successfully\n");
}

module_init(kallsyms_lookup_init) module_exit(kallsyms_lookup_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL");
