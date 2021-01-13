#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/param.h>

#define PROC_NAME "jiff"
#define BUFFER_SIZE 128


ssize_t proc_read(struct file* file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations ops = {
	.owner = THIS_MODULE,
	.read = proc_read,
};

int proc_init(void)
{
	proc_create(PROC_NAME, 0666, NULL, &ops);
	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	return 0;
}

void proc_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

ssize_t proc_read(struct file* file, char __user *usr_buf, size_t count, loff_t *pos)
{
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int complete = 0;

	if (complete) {
		complete = 0;
		return 0;
	}
	complete = 1;
	sprintf(buffer, "Number of timer ticks since startup: %lu\n", jiffies);
	rv = sprintf(buffer,buffer);
	copy_to_user(usr_buf, buffer, rv);

	return rv;
}
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("Mahmoud Abd Al-Ghany");
