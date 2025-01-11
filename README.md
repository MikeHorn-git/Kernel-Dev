[![kernelnewbies](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Description

A [Loadable Kernel Module](https://en.wikipedia.org/wiki/Loadable_kernel_module) (LKM) programming environment that utilizes Vagrant with support for Libvirt (QEMU) and VirtualBox.
Includes sample LKMs designed with a focus on rootkit functionality.

# Requirements

* Provider (Libvirt or Virtualbox)
* Vagrant

# Build

```bash
git clone https://github.com/MikeHorn-git/Kernel-Dev.git
cd Kernel-Dev
```

## Libvirt

```bash
make build
```

## Virtualbox

```bash
make build
```

# Makefile

```Makefile
Usage: make <target>
Targets:
help         Display this help message
base         Deploy base VM
build        Deploy VM and build kernel with defconfig
custom       Deploy VM with dotfiles and miscs tools
all          Execute base, build and custom command
format       Format all .c, .h files with clang-format
clean        Clean up VM images
```

# Module

## Features

> [!Important]
> This repository is for educational and research purposes only

* Kprobe-based hide LKM
* Basic openrc persistence
* Basic local revshell

## List

* file_hide
* hello
* lkm_hide
* persistence
* pid_hide
* port_hide
* revshell
