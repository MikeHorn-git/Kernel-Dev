obj-m := hello.o
all:
	make -C ../../linux-6.11 M=$(PWD) modules
clean:
	make -C ../../linux-6.11 M=$(PWD) clean
