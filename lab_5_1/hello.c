
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

// SPDX-License-Identifier: GPL-2.0
MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat_count = 1;

module_param(repeat_count, uint, 0644);
MODULE_PARM_DESC(repeat_count, "Кількість разів для виведення 'Hello, world!' (за замовчуванням 1)");

struct my_data {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(my_list);

static int __init hello_init(void)
{
    if (repeat_count > 10) {
        pr_err("Неприпустиме значення repeat_count (>10): %d\n", repeat_count);
        BUG_ON(1); // Викликати BUG_ON для неприпустимого repeat_count
        return -EINVAL;
    }

    if (repeat_count == 0 || (repeat_count >= 5 && repeat_count <= 10)) {
        pr_warn("Попередження: значення repeat_count дорівнює 0 або знаходиться між 5 і 10 (%d)\n", repeat_count);
    }

    struct my_data *data;
    int i;

    for (i = 0; i < repeat_count; ++i) {
        // Simulate kmalloc() failure
        data = NULL;

        if (!data) {
            pr_err("Не вдалося виділити пам'ять\n");
            data = ERR_PTR(-ENOMEM); // Примусове внесення помилки
            BUG_ON(IS_ERR(data)); // Викликати BUG_ON для помилкового kmalloc
            return -ENOMEM;
        }

        INIT_LIST_HEAD(&data->list);
        data->time = ktime_get();

        list_add(&data->list, &my_list);

        pr_emerg("Hello, world!\n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *data, *tmp;

	list_for_each_entry_safe(data, tmp, &my_list, list) {
		pr_info("Час: %lld нс\n", ktime_to_ns(data->time));
		list_del(&data->list);
		kfree(data);
	}
}

module_init(hello_init);
module_exit(hello_exit);

