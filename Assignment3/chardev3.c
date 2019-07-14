#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shweta Aswani");
MODULE_DESCRIPTION("Character Device");


/*
 *  chardev3.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

/* 
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);

static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int rindex = 0, windex = 0;
static int empty_slots_available;
char *buffer1;

#define SUCCESS 0
#define DEVICE_NAME "chardev3"    /* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80        /* Max length of the message from the device */

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;        /* Major number assigned to our device driver */
static int Device_Open = 0;    /* Is device open?*/

static int buffersize;
module_param(buffersize,int,0);
                 /* Used to prevent multiple access to device */
static char msg[BUF_LEN];    /* The msg the device will give when asked */
static char *msg_Ptr;
static DEFINE_SEMAPHORE(semFull);
static DEFINE_SEMAPHORE(semEmpty);
static DEFINE_SEMAPHORE(mutex);


static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "buffersize: %d\n", buffersize);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    buffer1 = kmalloc(buffersize*sizeof(char*), GFP_KERNEL);
    if(!buffer1)
        {printk(KERN_INFO "Could not allocate memory! \n");}

    sema_init(&semFull, 0);
    sema_init(&semEmpty, buffersize);
    sema_init(&mutex, 1);
   
   

    return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
    kfree(buffer1);
    /*
     * Unregister the device
     */
    unregister_chrdev(Major, DEVICE_NAME);
   
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{


   
    Device_Open++;
   
    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;        /* We're now ready for our next caller */

    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     */
   

    return 0;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,    /* see include/linux/fs.h   */
               char *buffer,    /* buffer to fill with data */
               size_t length,    /* length of the buffer     */
               loff_t * offset)
{

down_interruptible(&semFull);
down_interruptible(&mutex);

rindex %= buffersize;


        copy_to_user(&buffer, &buffer1+sizeof(int)*rindex, 4);
   

    ++rindex;
   


up(&mutex);
up(&semEmpty);


return length;
}

/* 
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{


    down_interruptible(&semEmpty);
    down_interruptible(&mutex);
   
    windex %= buffersize;

   
    copy_from_user(&buffer1+sizeof(int)*windex, &buff, 4); //copy from user space to kernel space, buffer1 is for the kernel and buff is the user buffer
   

    ++windex;
       

    up(&mutex);
    up(&semFull);


return len;

}
