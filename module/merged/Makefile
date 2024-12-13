obj-m = main.o
KDIR := ../../linux-6.11
EXTRA_CFLAGS += -DDEBUG

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean