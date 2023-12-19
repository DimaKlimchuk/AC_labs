#ifndef HELLO1_H
#define HELLO1_H

#include <linux/list.h>
#include <linux/ktime.h>

struct my_data {
    struct list_head list;
    ktime_t time_before;
    ktime_t time_after;
};

extern void print_hello(unsigned int repeat_count);

#endif
