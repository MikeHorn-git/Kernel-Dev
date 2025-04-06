KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += port_hide.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/port_hide.ko
	@sudo depmod

modprobe:
	@sudo cp port_hide.ko /lib/modules/$(KERN_VERSION)/port_hide.ko
	@sudo depmod
