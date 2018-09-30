obj-m := test.o

KERNELDIR ?=/mnt/hgfs/ubuntu_win7/9x25/mr301/linux/linux-at91-linux-2.6.39-at91
PWD ?= $(shell pwd)

modules:	
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-  -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD) 

clean:
	rm -rf *.ko *.o *.mod.c Module.symvers *.order
