// chirisco.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iván Chavero");
MODULE_DESCRIPTION("Un modulito chirisco");
MODULE_VERSION("1.0");


#define DEVICE_NAME "chirisco"
#define BUFFER_SIZE 1024

static int major_number;
static int device_open_count = 0;
static char device_buffer[BUFFER_SIZE];


static int chirisco_device_open(struct inode *inode, struct file *file);
static int chirisco_device_close(struct inode *inode, struct file *file);
static ssize_t chirisco_device_read(struct file *filep, char *buffer, size_t len, loff_t *offset);

// Define file operations structure
static struct file_operations fops = {
    .open = chirisco_device_open,
    .release = chirisco_device_close,
    .read = chirisco_device_read
};


// Open function
static int chirisco_device_open(struct inode *inode, struct file *file) {
    device_open_count++;
    printk(KERN_INFO "Se abrió el dispositivo: %s\n", DEVICE_NAME);
    return 0;
}

    //printk(KERN_INFO "Cha-la head-cha-la No importa lo que suceda Siempre el ánimo mantendré: %s\n", device_open_count);
// Close function
static int chirisco_device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Sonreiré el día de ho-ho-ho-ho-hoy\n");
    return 0;
}


static int __init chirisco_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_module: Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "Cargando Módulo Chirisco!\n");
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major_number);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major_number);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");
    return 0;
}

static void __exit chirisco_exit(void) {
    printk(KERN_INFO "Adiós zoquetes!!!\n");
}


/* Device read function */
/*static ssize_t chirisco_device_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    if (*offset >= BUFFER_SIZE) return 0;

    if (len + *offset > BUFFER_SIZE) len = BUFFER_SIZE - *offset;

    if (copy_to_user(buffer, device_buffer + *offset, len) != 0) return -EFAULT;

    *offset += len;
    bytes_read = len;
    printk(KERN_INFO "chardev: Sent %d bytes to the user\n", bytes_read);
    return bytes_read;
}*/


static ssize_t chirisco_device_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    char *message = "Cha-la head-cha-la No importa lo que suceda Siempre el ánimo mantendré\n";
    int message_len = strlen(message);
    errors = copy_to_user(buffer, message, message_len);
    //printk(KERN_INFO "user_message: %s\n", message);
    return errors == 0 ? message_len : -EFAULT;
}

module_init(chirisco_init);
module_exit(chirisco_exit);

