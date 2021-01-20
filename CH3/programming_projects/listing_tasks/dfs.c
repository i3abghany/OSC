#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

static void dfs(struct task_struct *curr, size_t curr_level);

static int list_init(void)
{
	printk("DFS-tree process list module initialized.\n");
	dfs(&init_task, 0);
	return 0;
}

static void list_exit(void)
{
	printk("DFS-tree process list module unloaded.\n");
}

static void dfs(struct task_struct *curr, size_t curr_level) 
{
	size_t i;
	struct list_head *list; 
	struct task_struct *task; 

	if (curr == NULL)
		return;

	if (curr_level >= 100)
		return;

	for (i = 0; i < curr_level; i++)
		printk("  ");
	printk("[%s], pid = [%d]\n", curr->comm, curr->pid);

	list_for_each(list, &curr->children) {
		task = list_entry(list, struct task_struct, sibling);
		dfs(task, curr_level + 1);
	}
}

MODULE_LICENSE("GPL");

module_init(list_init);
module_exit(list_exit);
