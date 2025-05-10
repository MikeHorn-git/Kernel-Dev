// SPDX-License-Identifier: GPL-2.0-only

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/module.h>

#ifdef DEBUG
#define dbg_print(fmt, ...) pr_info(fmt, ##__VA_ARGS__)
#else
#define dbg_print(fmt, ...) /* No-op */
#endif

static int persistence(void)
{
	struct file *file;
	char *data = kasprintf(GFP_KERNEL, "%s\n", THIS_MODULE->name);
	char *path = "/etc/modules-load.d/modules.conf";
	loff_t pos = 0; // File offset
	ssize_t written;

	// Check file exist
	file = filp_open(path, O_RDONLY, 0);
	if (!IS_ERR(file)) {
		dbg_print("modules.conf already exists");
		filp_close(file, NULL);
		kfree(data);
		return -1;
	}

	// Create or Open the file
	dbg_print("Open file");
	file = filp_open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(file)) {
		dbg_print("Failed to open file");
		kfree(data);
		return -1;
	}

	// Write data
	dbg_print("Write file");
	written = kernel_write(file, data, strlen(data), &pos);
	if (written < 0) {
		dbg_print("Failed to write file");
		filp_close(file, NULL);
		kfree(data);
		return -1;
	}

	filp_close(file, NULL);
	kfree(data);
	return 0;
}

static int __init persistence_init(void)
{
	int ret;

	dbg_print("persistence loaded\n");
	ret = persistence();

	if (ret == 0)
		dbg_print("openrc persistence loaded");
	else {
		dbg_print("openrc persistence failed");
	}

	return 0;
}

static void __exit persistence_exit(void)
{
	dbg_print("persistence exit successfully\n");
}

module_init(persistence_init) module_exit(persistence_exit)
	MODULE_AUTHOR("MikeHorn-git");
MODULE_DESCRIPTION("persistence");
MODULE_LICENSE("GPL");
