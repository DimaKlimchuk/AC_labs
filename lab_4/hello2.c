#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "inc/hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello2 module in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat_count = 1;

module_param(repeat_count, uint, 0644);
MODULE_PARM_DESC(repeat_count, "Number of times to print 'Hello, world!' (default 1)");

static int __init hello2_init(void)
{
    pr_info("Hello2 module initialized\n");
    print_hello(repeat_count);
    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("Hello2 module exited\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

