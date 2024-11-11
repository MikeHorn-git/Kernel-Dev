// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://github.com/humzak711
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/moduleparam.h>

typedef unsigned long (*kallsyms_ln_t)(const char *name);

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name",
};

/* Variable to hold the symbol name we want to look up */
static char *symbol_name = "sys_call_table"; // Default symbol

/* Register 'symbol_name' as a module parameter */
module_param(symbol_name, charp, 0644);
MODULE_PARM_DESC(symbol_name, "Symbol name to look up with kallsyms");

static int __init kallsyms_lookup_init(void)
{
	/* Register the kprobe to get the address of kallsyms_lookup_name */
	register_kprobe(&kp);
	kallsyms_ln_t kallsyms_ln = (kallsyms_ln_t)kp.addr;
	unregister_kprobe(&kp);

	/* Use the symbol_name parameter to look up the symbol */
	if (kallsyms_ln) {
		unsigned long address = kallsyms_ln(symbol_name);
		pr_info("%s: Address of %s is: %lx\n", __func__, symbol_name,
			address);
	} else {
		pr_err("Failed to find kallsyms_lookup_name function\n");
	}

	return 0;
}

static void __exit kallsyms_lookup_exit(void)
{
	pr_info("kallsyms_lookup exit successfully\n");
}

module_init(kallsyms_lookup_init) module_exit(kallsyms_lookup_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("kallsyms_lookup_name");
MODULE_LICENSE("GPL");
