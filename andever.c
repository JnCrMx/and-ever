#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "forever"
#define MESSAGE_1 "forever "
#define MESSAGE_1_LEN 8
#define MESSAGE_2 "and ever "
#define MESSAGE_2_LEN 9

static int andever_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = generic_file_open(inode, file);
	if (unlikely(ret))
		return ret;
	
	file->private_data = 0;
	
	return 0;
}

static int andever_release(struct inode *ignored, struct file *file)
{
	return 0;
}

static ssize_t andever_read(struct file *file, char __user *buf, size_t len, loff_t *pos)
{
	if(file->private_data)
	{
		copy_to_user(buf, MESSAGE_2, MESSAGE_2_LEN);
		return MESSAGE_2_LEN;
	}
	else
	{
		file->private_data = 1;
	
		copy_to_user(buf, MESSAGE_1, MESSAGE_1_LEN);
		return MESSAGE_1_LEN;
	}
}
	
static const struct file_operations andever_fops = {
	.owner = THIS_MODULE,
	.open = andever_open,
	.release = andever_release,
	.read = andever_read
};

static struct miscdevice andever_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &andever_fops,
};

static int __init ever_init(void) 
{	
	int ret;
	
	ret = misc_register(&andever_misc);
	if (unlikely(ret)) {
		pr_err("failed to register misc device!\n");
		return ret;
	}
	
	return 0;
}

static void __exit ever_exit(void)
{
	misc_deregister(&andever_misc);
}
 
module_init(ever_init);
module_exit(ever_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("JCM");
MODULE_DESCRIPTION("forever and ever and ever and ever");
MODULE_VERSION("0.01");
