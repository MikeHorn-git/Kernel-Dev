// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/moduleparam.h>

static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name",
};

static char *symbol_name = "sys_call_table";
module_param(symbol_name, charp, 0644);
MODULE_PARM_DESC(symbol_name, "Symbol name to look up with kallsyms");

static int __init kallsyms_lookup_init(void)
{
	register_kprobe(&kp);

	if (kp.addr) {
		unsigned long address =
			((unsigned long (*)(const char *))kp.addr)(symbol_name);
		pr_info("Address of %s is: %lx\n", symbol_name, address);
	} else {
		pr_err("Failed to find kallsyms_lookup_name function\n");
	}

	unregister_kprobe(&kp);
	return 0;
}

static void __exit kallsyms_lookup_exit(void)
{
	pr_info("kallsyms_lookup exited successfully\n");
}

module_init(kallsyms_lookup_init) module_exit(kallsyms_lookup_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL");
