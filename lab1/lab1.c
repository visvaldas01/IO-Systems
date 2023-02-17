#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Irina Kurnosova, Vsevolod Antonov");
MODULE_DESCRIPTION("IO");
MODULE_VERSION("1.0");

#define VAR "var5"
#define DRIVER_NAME VAR
#define DRIVER_CLASS "IO_class"
#define READ_BUF_SIZE 15

static dev_t my_device;
static struct class *my_class;
static struct cdev my_device_struct;
static struct proc_dir_entry* proc_entry;


int total_read_letters = 0;

static ssize_t p_read(struct file *file, char __user * ubuf, size_t count, loff_t* ppos) {
	return -1;
}

/*static ssize_t p_write(struct file *file, const char __user * ubuf, size_t count, loff_t* ppos) {
	return -1;
}*/

static ssize_t dev_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
	static char my_buf[READ_BUF_SIZE];
	size_t read_next = count < READ_BUF_SIZE ? count : READ_BUF_SIZE;
	if (copy_from_user(my_buf, ubuf, read_next) != 0) {
		return -EFAULT;
	}	
	for (int i = 0; i < read_next; i++) {
		total_read_letters += (('A' <= my_buf[i]) && (my_buf[i] <= 'Z')) || (('a' <= my_buf[i]) && (my_buf[i] <= 'z')) ? 1 : 0;
	}
	*ppos += read_next;
	return read_next;
}

static ssize_t dev_read(struct file *file, char __user *ubuf, size_t count, loff_t* ppos) {
	return -1;
}

static struct proc_ops proc_fops = {
	.proc_read = p_read,
//	.proc_write = p_write
};

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write
};

static int __init io_init(void) {
	if (alloc_chrdev_region(&my_device, 0, 1, DRIVER_NAME) < 0) {
                printk("Device could not be allocated\n");
                return -1;
        }
        printk("my_driver device number was registered, Major:%d, Minor:%d\n", my_device >> 20, my_device & 0xffff);

        /* Create device class */
        if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
                printk("Device class can't de created\n");
                unregister_chrdev_region(my_device, 1);
	        return -1;
        }

        /* Create device file */
        if (device_create(my_class, NULL, my_device, NULL, DRIVER_NAME) == NULL) {
                printk("Can't create device file\n");
 	        class_destroy(my_class);
		unregister_chrdev_region(my_device, 1);
                return -1;
	}

        /* Initialize device file */
        cdev_init(&my_device_struct, &dev_fops);

        /* Register device to kernel */
        if (cdev_add(&my_device_struct, my_device, 1) == -1) {
                printk("Registering of device to kernel failed\n");
                device_destroy(my_class, my_device);
		class_destroy(my_class);
                unregister_chrdev_region(my_device, 1);
                return -1;
        }

        printk("\nDevice successfully registered! Device name: %s\n", DRIVER_NAME);

	proc_entry = proc_create(VAR, S_IRUSR|S_IRGRP|S_IROTH, NULL, &proc_fops);

        return 0;
}

static void __exit io_exit(void) {
	proc_remove(proc_entry);
        cdev_del(&my_device_struct);
        device_destroy(my_class, my_device);
        class_destroy(my_class);
        unregister_chrdev_region(my_device, 1);
        printk(KERN_INFO "Module is finished\n");
}


module_init(io_init);
module_exit(io_exit);
