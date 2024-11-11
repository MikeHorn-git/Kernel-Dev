FILES := '*.c' '*.h'
FORMAT := clang-format

OVA_FILE := kernel.ova
QCOW2_IMG := kernel.qcow2

VM_NAME := Kernel

.DEFAULT_GOAL := help

help:
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "  help         Display this help message"
	@echo "  build        Deploy VM and build kernel with defconfig"
	@echo "  convert      Convert $(VM_NAME) VM to QCOW2 format"
	@echo "  deploy       Deploy VM with utils"
	@echo "  format       Format all .c, .h files with clang-format"
	@echo "  clean        Clean up VM images"
	@echo "  prune        Destroy Vagrant VM"
	@echo "  distclean    Execute clean and prune command"

build:
	@command -v vagrant >/dev/null 2>&1 || { echo "Vagrant is not installed."; exit 1; }
	@vagrant --kernel up || { echo "Failed to deploy with Vagrant."; exit 1; }

convert:
	VBoxManage export $(VM_NAME) -o $(OVA_FILE) --ovf10
	tar -xvf $(OVA_FILE)
	qemu-img convert -f vmdk -O qcow2 $(OVA_FILE) $(QCOW2_IMG) || { echo "Failed to convert image."; exit 1; }

deploy:
	@command -v vagrant >/dev/null 2>&1 || { echo "Vagrant is not installed."; exit 1; }
	@vagrant up || { echo "Failed to deploy with Vagrant."; exit 1; }

format:
	git ls-files $(FILES) | xargs $(FORMAT) -i

clean:
	rm -rf $(OVA_FILE) $(QCOW2_IMG) *.ovf *.vmdk

prune:
	@vagrant destroy -f
	@vagrant global-status --prune

distclean: clean prune

.PHONY: help build convert deploy format clean prune distclean
