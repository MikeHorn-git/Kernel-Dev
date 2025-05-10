KERN_VERSION = $(shell uname -r)
PWD := $(shell pwd)

obj-m += hello.o
obj-m += hello2.o

all:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERN_VERSION)/build M=$(PWD) clean
