#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
struct proc_dir_entry *tempdir;//声明全局变量temp_dir指针
char pid[10] = {'\0'};//pid变量
static int hello_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m,"%s",pid);
	return 0;
}

static int hello_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, hello_proc_show, NULL);//打开文件操作
}
static int myproc_write(struct file *file,const char __user *buffer,unsigned long count,void *data){
	unsigned long count2 = count;
	if(count2 >= sizeof(pid))
		count2 = sizeof(pid)-1;
	if(copy_from_user(pid,buffer,count2))//将用户空间指针buffer的count2数量的数据复制到内核空间pid数组
		return -EFAULT;
		return count;
}
static const struct file_operations hello_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= hello_proc_open,
	.read		= seq_read,
	.write	    	= myproc_write,//定义写入proc文件的写回调函数
	.llseek		= seq_lseek,
	.release	= single_release,
}; 
static int __init hello_proc_init(void)
{
	int rv = 0;	
	tempdir = proc_mkdir("sigkill",NULL);//创建sigkill目录在proc下
	if(tempdir == NULL){
		rv = -ENOMEM;
		return rv;			
	}
	proc_create("sigkill",0,tempdir,&hello_proc_fops);//再创建sigkill文件
	return 0;
}
static void __exit hello_proc_exit(void)
{		
	remove_proc_entry("sigkill",tempdir);//删除文件
	remove_proc_entry("sigkill",NULL);//删除目录
	printk("GoodBye kernel\n");
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
