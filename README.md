[![kernelnewbies](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Description

A [Linux Kernel](https://en.wikipedia.org/wiki/Loadable_kernel_module) developer environments.  

## Requirements

```bash
VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1 vagrant plugin install vagrant-reload
```

### Libvirt

```bash
VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1 vagrant plugin install vagrant-libvirt
```

### Virtualbox

Supported by default.

## Images

* Alpine
* Arch
* Debian
* Gentoo
* Ubuntu

## Provider

* Libvirt
* Virtualbox

---

## Build

```bash
git clone https://github.com/MikeHorn-git/Kernel-Dev.git
cd Kernel-Dev
```

### Libvirt

```bash
make build
```

### Virtualbox

The Vagrantfile use libvirt provider by default.
Remove ```ENV['VAGRANT_DEFAULT_PROVIDER'] = 'libvirt'```.

```bash
sed -i "/ENV\['VAGRANT_DEFAULT_PROVIDER'\] = 'libvirt'/d" Vagrantfile
make build
```

## Makefile

```Makefile
Usage: make <target>
Targets:
  help         Display this help message
  base         Deploy base VM
  build        Deploy VM and build kernel with defconfig
  custom       Deploy VM miscs tools
  all          Execute base, build and custom command
  format       Format all .c, .h files with clang-format
```

## Vagrant Provisionning

## Default

* Update & Upgrade distros
* Mandatory kernel-build packages
* System tweaks:
   * `CTRL+L` clears the terminal.
   * Sets `TERM=xterm-256color` to prevent terminal type errors.

## VAGRANT_KERNEL

Use ```make build``` or export `VAGRANT_KERNEL=true` in env:

* Kernel Download: Linux kernel version `6.11`.
* Kernel Compilation and Installation:
   * Extracts and compiles the kernel.
   * Installs kernel modules and the kernel itself.
* Initramfs Creation
* Bootloader Update

## VAGRANT_CUSTOM

Use ```make custom``` or export `VAGRANT_CUSTOM=true` in env:

* Oh-My-Bash (OMB): Installs Oh-My-Bash.
* Rsyslog:
   * Installs `rsyslog`.
   * Starts the service and enables it at boot.

---

# Src

* hello syscall
* [jiffy](https://en.wikipedia.org/wiki/Jiffy_(time)#Computing) syscall
* Userland test

# Module

> [!Important]
> These samples are for educational and research purposes only.

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
