#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

static int list_init(void)
{
	struct task_struct *tsk;
	int i = 0;
	printk("List-Process kernel module initialized.\n");
	printk("%-5d%-7s      %-7s       %-7s       %-7s       %-7s       %-7s\n",
	i, "S", "UID", "PID", "PPID", "PRI", "CMD");
	for_each_process(tsk) {
		printk("%-5d%-7ld      %-7u      %-7d      %-7d      %-7u      %-7s\n",
			i, tsk->state, tsk->loginuid.val, tsk->pid, tsk->real_parent->pid,
			tsk->rt_priority, tsk->comm);
		i++;
	}
	return 0;
}

static void list_exit(void)
{
	printk("List-Process kernel module deleted.\n");
}

module_init(list_init);
module_exit(list_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List-Pid");
MODULE_AUTHOR("Mahmoud Abd Al-Ghany");

