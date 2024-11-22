KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += revshell_local.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/revshell_local.ko
	@sudo depmod

modprobe:
	@sudo cp revshell_local.ko /lib/modules/$(KERN_VERSION)/revshell_local.ko
	@sudo depmod
