QCOW2_IMG := kernel.qcow2
DISK_IMG := disk.img
OVA_FILE := kernel.ova
VM_NAME := Kernel

.DEFAULT_GOAL := help

help:
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "  help         Display this help message"
	@echo "  convert      Convert $(VM_NAME) VM to QCOW2 format"
	@echo "  deploy       Deploy using Vagrant"
	@echo "  clean        Clean up generated files"
	@echo "  prune        Destroy Vagrant VM"

convert:
	VBoxManage export $(VM_NAME) -o $(OVA_FILE) --ovf10
	tar -xvf $(OVA_FILE)
	qemu-img convert -f vmdk -O qcow2 $(OVA_FILE) $(QCOW2_IMG) || { echo "Failed to convert image."; exit 1; }

deploy:
	@command -v vagrant >/dev/null 2>&1 || { echo "Vagrant is not installed."; exit 1; }
	vagrant --kernel up || { echo "Failed to deploy with Vagrant."; exit 1; }

clean:
	rm -rf $(OVA_FILE) $(QCOW2_IMG) *.ovf *.vmdk

prune:
	@vagrant destroy -f
	@vagrant global-status --prune

.PHONY: help convert deploy clean prune
