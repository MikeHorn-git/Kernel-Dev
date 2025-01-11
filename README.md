[![kernelnewbies](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Description

My [Linux Kernel](https://en.wikipedia.org/wiki/Loadable_kernel_module) developer environments.  
Use Vagrant with Libvirt (QEMU) and VirtualBox providers and Alpine image.  
Include [in-tree](https://github.com/MikeHorn-git/Kernel-Dev#src) and [out-of-tree](https://github.com/MikeHorn-git/Kernel-Dev#module) code.

# Requirements

## Libvirt

```bash
export VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1
vagrant plugin install vagrant-libvirt
```

## Virtualbox

Supported by default

---

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

The Vagrantfile use libvirt provider by default.
Remove ```ENV['VAGRANT_DEFAULT_PROVIDER'] = 'libvirt'```

```bash
sed -i "/ENV\['VAGRANT_DEFAULT_PROVIDER'\] = 'libvirt'/d" Vagrantfile
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

# Src

In-tree dev.  
Custom syscalls.

## Features

* hello syscall
* [jiffy](https://en.wikipedia.org/wiki/Jiffy_(time)#Computing) syscall
* Userland test

# Module

Out-of-tree dev.  
Rootkit samples.

> [!Important]
> This samples is for educational and research purposes only

* Kprobe-based hide LKM
* Openrc persistence
* Local revshell

## List

* file_hide
* hello
* lkm_hide
* persistence
* pid_hide
* port_hide
* revshell
