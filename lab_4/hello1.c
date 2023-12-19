#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "inc/hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello1 module in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(my_list);

void print_hello(unsigned int repeat_count) {
    struct my_data *data;
    int i;

    for (i = 0; i < repeat_count; ++i) {
        data = kmalloc(sizeof(struct my_data), GFP_KERNEL);
        if (!data) {
            pr_err("Failed to allocate memory\n");
            return;
        }

        INIT_LIST_HEAD(&data->list);
        data->time_before = ktime_get();

        list_add(&data->list, &my_list);

        pr_info("Hello, world!\n");

        data->time_after = ktime_get();
        pr_info("Time taken for print_hello: %lld ns\n", ktime_to_ns(ktime_sub(data->time_after, data->time_before)));
    }
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("Hello1 module initialized\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct my_data *data, *tmp;

    pr_info("Printing the times:\n");

    list_for_each_entry_safe(data, tmp, &my_list, list) {
        pr_info("Time: %lld ns\n", ktime_to_ns(ktime_sub(data->time_after, data->time_before)));
        list_del(&data->list);
        kfree(data);
    }

    pr_info("Hello1 module exited\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

