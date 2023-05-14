#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(my_mutex_lock, int, mutex_id)
{
    printk("Mutex lock syscall invoked for mutex ID: %d\n", mutex_id);
    return 0;
}

