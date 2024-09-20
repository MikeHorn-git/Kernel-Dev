#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
// https://devarea.com/linux-kernel-development-kernel-module-parameters/
// https://tldp.org/LDP/lkmpg/2.6/html/x323.html
// https://stackoverflow.com/questions/12311867/how-to-call-exported-kernel-module-functions-from-another-module

extern void hello;

static __init int hello_init(void)
{
    pr_info("Hello, world!\n");
    return 0;
}

static __exit void hello_exit(void)
{
    pr_info("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module 2");
