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
	@echo "  base         Deploy base VM"
	@echo "  build        Deploy VM and build kernel with defconfig"
	@echo "  custom       Deploy VM with dotfiles and miscs tools"
	@echo "  all          Execute base, build and custom command"
	@echo "  convert      Convert $(VM_NAME) VM to QCOW2 format"
	@echo "  format       Format all .c, .h files with clang-format"
	@echo "  clean        Clean up VM images"
	@echo "  prune        Destroy Vagrant VM"
	@echo "  distclean    Execute clean and prune command"

base:
	@vagrant up
	@vagrant halt

build:
	@export VAGRANT_KERNEL=true && vagrant up --provision
	@vagrant halt

custom:
	@export VAGRANT_CUSTOM=true && VAGRANT_IDE=true && vagrant up --provision
	@vagrant halt

all: base build custom

convert:
	VBoxManage export $(VM_NAME) -o $(OVA_FILE) --ovf10
	tar -xvf $(OVA_FILE)
	qemu-img convert -f vmdk -O qcow2 $(OVA_FILE) $(QCOW2_IMG) || { echo "Failed to convert image."; exit 1; }

format:
	git ls-files $(FILES) | xargs $(FORMAT) -i

clean:
	rm -rf $(OVA_FILE) $(QCOW2_IMG) *.ovf *.vmdk

prune:
	@vagrant destroy -f
	@vagrant global-status --prune

distclean: clean prune

.PHONY: help base build custom all convert format clean prune distclean
