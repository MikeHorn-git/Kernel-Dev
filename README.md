[![image](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Requirements

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
  format       Format all .c, .h files with clang-format
  clean        Clean up VM images
  prune        Destroy Vagrant VM
  distclean    Execute clean and prune command
```

# Modules

* file_hide
* hello
* lkm_hide
* persistence
* pid_hide
* port_hide
* revshell
