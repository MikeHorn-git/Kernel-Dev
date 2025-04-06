// https://devarea.com/linux-kernel-development-kernel-module-parameters/
// https://tldp.org/LDP/lkmpg/2.6/html/x323.html
// https://stackoverflow.com/questions/12311867/how-to-call-exported-kernel-module-functions-from-another-module
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

extern void hello(void);

/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

static char *name = "KERNEL MODULE DEV";
module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "A name parameter to customize the hello message");

static int __init hello2_init(void)
{
	pr_info("Hello2 module: Custom parameter: %s\n", name);
	hello();
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("Hello2 module: exiting\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module 2");
