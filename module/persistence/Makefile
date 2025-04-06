KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += persistence.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/persistence.ko
	@sudo depmod

modprobe:
	@sudo cp persistence.ko /lib/modules/$(KERN_VERSION)/persistence.ko
	@sudo depmod
