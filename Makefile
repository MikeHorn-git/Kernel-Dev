FILES := '*.c' '*.h'
FORMAT := clang-format

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
	@echo "  format       Format all .c, .h files with clang-format"
	@echo "  clean        Clean up VM images"

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

format:
	git ls-files $(FILES) | xargs $(FORMAT) -i

clean:
	@vagrant destroy -f
	@vagrant global-status --prune

.PHONY: help base build custom all format clean
