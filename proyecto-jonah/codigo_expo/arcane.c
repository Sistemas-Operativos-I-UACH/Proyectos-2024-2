// arcane.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonah GGC");
MODULE_DESCRIPTION("Ekko MVP");
MODULE_VERSION("2.7");


#define DEVICE_NAME "arcane"
#define BUFFER_SIZE 1024

static int major_number;
static int device_open_count = 0;
static char device_buffer[BUFFER_SIZE];


static int arcane_device_open(struct inode *inode, struct file *file);
static int arcane_device_close(struct inode *inode, struct file *file);
static ssize_t arcane_device_read(struct file *filep, char *buffer, size_t len, loff_t *offset);

int already_read = 0;
static int line_index = 0;
static const char *lyrics[] = {
    "Je t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\n",
    "T'es la meilleure chose qui m'est arrivée\nMais aussi la pire chose qui m'est arrivée\nCe jour où je t'ai rencontrée, j'aurais peut-être préféré\nQue ce jour ne soit jamais arrivé (Arrivé)\nLa pire des bénédictions\nLa plus belle des malédictions\nDe toi, j'devrais m'éloigner\nMais comme dit le dicton :\n\"Plutôt qu'être seul, mieux vaut être mal accompagné\"\n",
    "Tu sais c'qu'on dit\nSois près d'tes amis les plus chers\nMais aussi\nEncore plus près d'tes adversaires\n",
    "Mais ma meilleure ennemie, c'est toi\nFuis-moi, le pire, c'est toi et moi\nMais si tu cherches encore ma voix\nOublie-moi, le pire, c'est toi et moi\n",
    "Pourquoi ton prénom me blesse\nQuand il se cache juste là dans l'espace ?\nC'est quelle émotion, la haine\nOu la douceur, quand j'entends ton prénom ?\n",
    "Je t'avais dit : \"Ne regarde pas en arrière\"\nLe passé qui te suit te fait la guerre\n",
    "Mais ma meilleure ennemie, c'est toi\nFuis-moi, le pire, c'est toi et moi\nMais ma meilleure ennemie, c'est toi\nFuis-moi, le pire, c'est toi et moi\n",
    "Je t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\nJe t'aime, je te quitte, je t'aime, je te quitte\n"
};
static int lyrics_count = sizeof(lyrics) / sizeof(lyrics[0]);

// Define file operations structure
static struct file_operations fops = {
    .open = arcane_device_open,
    .release = arcane_device_close,
    .read = arcane_device_read
};


// Open function
static int arcane_device_open(struct inode *inode, struct file *file) {
    device_open_count++;
    printk(KERN_INFO "Se abrió el dispositivo: %s\n", DEVICE_NAME);
    return 0;
}

// Close function
static int arcane_device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Vean Arcane!!!\n");
    return 0;
}


static int __init arcane_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_module: Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "Cargando Módulo arcane!\n");
    printk(KERN_INFO "Se le asigno el identificador %d. Para hablar con el dispositvo\n", major_number);
    printk(KERN_INFO "del controlador, ingrese lo siguiente en la terminal?\n");
    printk(KERN_INFO "'sudo mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major_number);
    printk(KERN_INFO "Elimine el dispositivo y el modulo al terminar.\n");
    printk(KERN_INFO "'sudo rmmod arcane'.\n");
    printk(KERN_INFO "'sudo rm /dev/arcane'.\n");
    return 0;
}

static void __exit arcane_exit(void) {
    printk(KERN_INFO "Je t'aime, je te quitte, je t'aime, je te quitte\n");
}

ssize_t arcane_device_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    if (!already_read) {
        int errors = 0;
        const char *message = lyrics[line_index];
        int message_len = strlen(message);

        errors = copy_to_user(buffer, message, message_len);
        already_read = 1;

        if (errors == 0) {
            line_index = (line_index + 1) % lyrics_count;
            return message_len;
        } else {
            return -EFAULT;
        }
    } else {
        already_read = 0;
        return 0;
    }
}

module_init(arcane_init);
module_exit(arcane_exit);

