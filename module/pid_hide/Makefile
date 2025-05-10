KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += pid_hide.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/pid_hide.ko
	@sudo depmod

modprobe:
	@sudo cp pid_hide.ko /lib/modules/$(KERN_VERSION)/pid_hide.ko
	@sudo depmod
