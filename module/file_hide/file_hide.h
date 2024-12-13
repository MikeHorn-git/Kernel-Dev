#ifndef FILE_HIDE_H
#define FILE_HIDE_H
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs);

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif


#endif //FILE_HIDE_H
