SH := bash

DISK_IMG := disk.img
QCOW2_IMG := disk.qcow2

KERN_VERSION := linux-6.11
KERN_URL := https://cdn.kernel.org/pub/linux/kernel/v6.x/$(KERN_VERSION).tar.xz
KERN_DIR := $(KERN_VERSION)

BUILD_SCRIPT := ./build
MODULE_DIR := ./src/module

# Use `@` to make commands quiet; use `VERBOSE=1` for a verbose output
QUIET := @
ifneq ($(VERBOSE),1)
	QUIET := @
endif

.DEFAULT_GOAL := help

help:
	@echo "Usage: make <target>"
	@echo "Targets:"
	@echo "  help         Display this help message"
	@echo "  build        Run the build script"
	@echo "  convert      Convert disk.img to QCOW2 format"
	@echo "  deploy       Download, extract, and configure kernel"
	@echo "  module       Build kernel modules"
	@echo "  share        Run QEMU with mount shared folder"
	@echo "  clean        Remove built kernel and module files"

build:
	$(QUIET)$(SH) $(BUILD_SCRIPT)

convert:
	$(QUIET)qemu-img convert -f raw -O qcow2 $(DISK_IMG) $(QCOW2_IMG) || { echo "Failed to convert image."; exit 1; }

deploy:
	@if [ -d "$(KERN_DIR)" ]; then \
		echo "Directory $(KERN_DIR) exists, skipping download."; \
	else \
		wget $(KERN_URL) || { echo "Failed to download kernel."; exit 1; }; \
		echo "Extracting kernel..."; \
		tar -xvf $(KERN_VERSION).tar.xz || { echo "Failed to extract kernel."; exit 1; }; \
		rm $(KERN_VERSION).tar.xz; \
		echo "Configuring kernel..."; \
		cd $(KERN_VERSION) && make defconfig || { echo "Kernel configuration failed."; exit 1; }; \
	fi

module:
	$(MAKE) -C $(MODULE_DIR) || { echo "Module build failed."; exit 1; }

share:
	$(QUIET)qemu-system-x86_64 -hda $(DISK_IMG) -nographic \
		-virtfs local,path="$(MODULE_DIR)",mount_tag=shared_folder,security_model=passthrough,id=host0 || \
		{ echo "Failed to start QEMU."; exit 1; }

clean:
	$(QUIET)rm -rf $(KERN_DIR) || echo "No kernel directory to clean."
	$(QUIET)$(MAKE) -C $(MODULE_DIR) clean || echo "Module clean failed or no files to clean."

.PHONY: help build convert deploy module share clean
