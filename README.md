[![image](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Overview

* Kernel-Dev environment powered by Vagrantfile.
* In-tree kernel patch at src/
* Userland tests in src/test
* LKM in module/

# Requirements

* [Getoptlong](https://github.com/ruby/getoptlong)
* Vagrant
* Virtualbox

# Usage

```Makefile
Usage: make <target>
Targets:
  help         Display this help message
  build        Deploy VM and build kernel with defconfig
  convert      Convert Kernel VM to QCOW2 format
  deploy       Deploy VM with utils
  clean        Clean up VM images
  prune        Destroy Vagrant VM
  distclean    Execute clean and prune command
```

# Vagrant

Vagrant commands are wrapped by the Makefile.

## Build

Execute ```bash vagrant --kernel up``` :

* Install Kernel utils
* Build and install Kernel

## Deploy

Execute ```bash vagrant up``` :

* Install Kernel utils

## Prune

Execute ```bash vagrant destroy -f; vagrant global-status --prune```

* Destroy the Kernel VM

# Module

TBD

# SRC

```bash
src
├── arch
│   └── x86
│       └── entry
│           └── syscalls
│               └── syscall_64.tbl
├── init
│   ├── custom
│   │   ├── hello.c
│   │   ├── Kconfig
│   │   └── uptime.c
│   ├── Kconfig
│   ├── main.c
│   └── Makefile
└── test
    ├── hello.c
    ├── Makefile
    └── uptime.c
```

* Modified files from kernel source tree
* Custom syscall created
* Userland call in test/

# To-Do

- [ ] Add libvirt (Qemu) support in Vagrantfile
