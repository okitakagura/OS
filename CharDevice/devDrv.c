#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/init.h"
#include "linux/types.h"
#include "linux/errno.h"
#include "linux/uaccess.h"
#include "linux/kdev_t.h"
#define MAX_SIZE 1024

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f);
static ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f);

static char message[MAX_SIZE] = "succeed!";
static int device_num = 0;//设备号
static int counter = 0;//计数用
static int mutex = 0;//互斥用
static char* devName = "myDevice";//设备名

struct file_operations pStruct =
{ open:my_open, release:my_release, read:my_read, write:my_write, };

// 注册模块
int init_module()
{
	int ret;
	/* 第一个参数是指新注册的设备的主设备号由系统分配，
	 * 第二个参数是新设备注册时的设备名字，
	 * 第三个参数是指向file_operations的指针，
	 * 当用设备号为0创建时，系统一个可以用的设备号创建模块 */
	ret = register_chrdev(0, devName, &pStruct);
	if (ret < 0)
	{
		printk("regist failure!\n");
		return -1;
	}
	else
	{
		printk("the device has been registered!\n");
		device_num = ret;
		printk("the virtual device's major number %d.\n", device_num);
		printk("------'mknod /dev/myDevice c %d 0'-------\n", device_num);
		printk("Use \"rmmod\" to remove the module\n");

		return 0;
	}
}

// 注销模块
void cleanup_module()
{
	unregister_chrdev(device_num, devName);
	printk("unregister it success!\n");
}

//打开设备
static int my_open(struct inode *inode, struct file *file)
{
        if(mutex)
                return -EBUSY;
        mutex = 1;//上锁
	printk("main  device : %d\n", MAJOR(inode->i_rdev));
	printk("slave device : %d\n", MINOR(inode->i_rdev));
	printk("%d times to call the device\n", ++counter);
	try_module_get(THIS_MODULE);
	return 0;
}

// 每次使用完后会释放
static int my_release(struct inode *inode, struct file *file)
{
	printk("Device released!\n");
	module_put(THIS_MODULE);
        mutex = 0;//开锁
	return 0;
}

//读操作
static ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
	if(copy_to_user(user,message,sizeof(message))) // 内核空间到用户空间的复制 
	{
		return -EFAULT;
	}
	return sizeof(message);
}

//写操作
static ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
	if(copy_from_user(message,user,sizeof(message))) //用户空间到内核空间的复制 
	{
		return -EFAULT;
	}
	return sizeof(message);
}


