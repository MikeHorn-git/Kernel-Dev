KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += lkm_hide.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/lkm_hide.ko
	@sudo depmod

modprobe:
	@sudo cp lkm_hide.ko /lib/modules/$(KERN_VERSION)/lkm_hide.ko
	@sudo depmod
