KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += kallsyms_lookup.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
	@sudo rm -f /lib/modules/$(KERN_VERSION)/kallsyms_lookup.ko
	@sudo depmod

modprobe:
	@sudo cp kprobe.ko /lib/modules/$(KERN_VERSION)/kallsyms_lookup.ko
	@sudo depmod
