obj-m += file_hide.o

KDIR := /home/prince2lu/Desktop/cours/rootkit/Kernel-Dev/linux-6.11
# PWD := /home/prince2lu/Desktop/cours/rootkit/Kernel-Dev/module/file_hide
EXTRA_CFLAGS += -DDEBUG

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean