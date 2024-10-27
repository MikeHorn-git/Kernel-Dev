[![image](https://github.com/user-attachments/assets/95a8f601-2a65-4563-b359-861b5b49eb8c)](https://kernelnewbies.org/)

# Overview

* Kernel-Dev environment powered by Vagrantfile.
* In-tree kernel patch at src/
* Userland tests in src/test
* LKM in module/

# Usage

```Makefile
Usage: make <target>
Targets:
  help         Display this help message
  convert      Convert Kernel VM to QCOW2 format
  deploy       Deploy using Vagrant
  clean        Clean up generated files
  prune        Destroy Vagrant VM
```
