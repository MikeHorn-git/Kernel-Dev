KERN_VERSION = linux-6.11
OBJ = hello.o

obj-m := $(OBJ)
all:
	make -C ../../$(KERN_VERSION) M=$(PWD) modules
clean:
	make -C ../../$(KERN_VERSION) M=$(PWD) clean
