#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

void hello(void);
void hello(void)
{
	pr_info("Hello from hello exported symbols\n");
}

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
MODULE_DESCRIPTION("Hello module");

EXPORT_SYMBOL_GPL(hello);
