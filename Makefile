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
	@echo "  custom       Deploy VM miscs tools"
	@echo "  ide          Deploy VM with custom neovim setup"
	@echo "  all          Execute base, build and custom command"
	@echo "  format       Format all .c, .h files with clang-format"

base:
	@vagrant up
	@vagrant halt

build:
	@export VAGRANT_KERNEL=true && vagrant up --provision
	@vagrant halt

custom:
	@export VAGRANT_CUSTOM=true && vagrant up --provision
	@vagrant halt

ide:
	@export VAGRANT_IDE=true && vagrant up --provision
	@vagrant halt

all: base build custom ide

format:
	git ls-files $(FILES) | xargs $(FORMAT) -i

.PHONY: help base build custom ide all format
