[![kernelnewbies](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Description

A [Linux Kernel](https://en.wikipedia.org/wiki/Loadable_kernel_module) developer environments.  
Use Vagrant with Libvirt (QEMU) and VirtualBox providers.  
Alpine-based image.  
Include [in-tree](https://github.com/MikeHorn-git/Kernel-Dev#src) and [out-of-tree](https://github.com/MikeHorn-git/Kernel-Dev#module) code.

# Requirements

## Libvirt

```bash
export VAGRANT_DISABLE_STRICT_DEPENDENCY_ENFORCEMENT=1
vagrant plugin install vagrant-libvirt
```

## Virtualbox

Supported by default.

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
Remove ```ENV['VAGRANT_DEFAULT_PROVIDER'] = 'libvirt'```.

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
  custom       Deploy VM miscs tools
  ide          Deploy VM with custom neovim setup
  all          Execute base, build and custom command
  format       Format all .c, .h files with clang-format
```

# Vagrant Provisionning

## Default

The default provisioning setup includes the following:

* Base system: Alpine Linux 3.18 upgraded to 3.21.
* Mandatory kernel-build packages:
* System tweaks:
   * `CTRL+L` clears the terminal.
   * Sets `TERM=xterm-256color` to prevent terminal type errors.
* System reboot.

## VAGRANT_KERNEL

Use ```make build``` or export `VAGRANT_KERNEL=true` in env:

* Kernel Download: Linux kernel version `6.11`.
* Kernel Compilation and Installation:
   * Extracts and compiles the kernel.
   * Installs kernel modules and the kernel itself.
* Initramfs Creation
* Bootloader Update
* System reboot.

## VAGRANT_CUSTOM

Use ```make custom``` or export `VAGRANT_CUSTOM=true` in env:

* Clang-Format: Installs `clang19-extra-tools`.
* Checkpatch.pl:
   * Downloads `checkpatch.pl` from the Linux kernel repository.
   * Sets executable permissions.
* Oh-My-Bash (OMB): Installs Oh-My-Bash.
* Rsyslog:
   * Installs `rsyslog`.
   * Starts the service and enables it at boot.

## VAGRANT_IDE

Use ```make ide``` or export `VAGRANT_IDE=true` in env:

* Neovim:
   * Installs `neovim`.
   * Installs custom neovim [dotfiles](https://github.com/MikeHorn-git/dotfiles/tree/main/nvim).
* Alias: Adds `alias vim=nvim` to `.bashrc`.

---

Export the `ENV` variables as needed before running `vagrant up`. Or use the provided [Makefile](https://github.com/MikeHorn-git/Kernel-Dev#makefile).

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
