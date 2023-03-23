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
#define READ_BUF_SIZE 1 << 12

static dev_t my_device;
static struct class *my_class;
static struct cdev my_device_struct;
static struct proc_dir_entry* proc_entry;


static char history[1024*16] = {0};
static size_t history_size = 0;

static bool is_letter(char c) {
	return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z'));
}

static ssize_t dev_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
	static char my_buf[READ_BUF_SIZE];
	size_t read_next = count < READ_BUF_SIZE ? count : READ_BUF_SIZE;
	if (copy_from_user(my_buf, ubuf, read_next) != 0) {
		return -EFAULT;
	}
	int i;
	int size = 0;
	for (i = 0; i < read_next; i++) {
		size += (is_letter(my_buf[i])) ? 1 : 0;
	}
	history_size += sprintf(history + history_size, "%d\n", size);
	*ppos += read_next;
	return read_next;
}

static ssize_t proc_read(struct file *file, char __user *ubuf, size_t count, loff_t* ppos) {
	if (*ppos > 0 || count < history_size) {
	    	return 0;
	}
	if (copy_to_user(ubuf, history, history_size) != 0) {
		return -EFAULT;
	}
	*ppos = count;
	printk("%s\n", history);
	return history_size;
}


static ssize_t dev_read(struct file *file, char __user *ubuf, size_t count, loff_t* ppos) {
	printk("%s", history);
	return 0;
}

static struct proc_ops proc_fops = {
	.proc_read = proc_read
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
        printk("my_driver device number was registered, Major: %d, Minor: %d\n", MAJOR(my_device), MINOR(my_device));

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
