/****************************************************************************/
/*                          kernel_spi API                                     */
/****************************************************************************/
/*
**
** MODULE AUTHOR(S):    yanglili
**
** MODULE TITLE:        kernel_spi.c
**
** MODULE INPUTS:       None
**
** MODULE FUNCTION:  
**
**
** MODULE OUTPUTS:
**
** DOCUMENT REFERENCES:
**
******************************************************************************
**  MODIFICATION HISTORY:
**  
**  Date           Person    CR       Change
**  --------       -------   ------   ------------------------------------ 
**  04/15/2016      yanglili          Create
**
*****************************************************************************/
#include "encryption.h"
	
dev_t                  devid;
static struct cdev     *spi_cdev; 
static int             spi_Major = 0;
static int             spi_Minor = 0;
static struct class    *spi_class;

static int spi_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int spi_close(struct inode *inode, struct file *file)
{
    return 0;
}


static struct file_operations spi_fops = { 
    .owner        = THIS_MODULE, 
	.open   =   spi_open,     
    .release = spi_close,
};


static int __init spi_init(void)
{
 
    int err;

    //初始化cdev 
    spi_cdev        = cdev_alloc(); 
    cdev_init(spi_cdev, &spi_fops); 
    spi_cdev->owner = THIS_MODULE;
	

    //动态获取主设备号(dev_t devid中包含"主设备号"和"次设备号"信息) 
    alloc_chrdev_region(&devid, 66, 1, "spi"); 
    spi_Major = MAJOR(devid);
    spi_Minor = MINOR(devid);

    //注册字符设备 (1)
    err = cdev_add(spi_cdev, devid, 1); 
    if (err) { 
        printk(KERN_NOTICE "Error %d adding device\n", err); 
        return -1; 
    }

    spi_class = class_create(THIS_MODULE, "spi_class1"); 
    if (IS_ERR(spi_class)) { 
        printk(KERN_INFO "create class error\n"); 
        return -1; 
    } 

	
    return 0;
}

static void __exit spi_exit(void)
{
    unregister_chrdev_region(devid, 1); 
    cdev_del(spi_cdev);
    device_destroy(spi_class, devid); 
    class_destroy(spi_class);
}

module_init(spi_init);    
module_exit(spi_exit);    
MODULE_AUTHOR("yanglili");    
MODULE_DESCRIPTION("spi"); 
MODULE_LICENSE("GPL");



