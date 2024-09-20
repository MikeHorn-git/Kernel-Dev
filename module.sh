#!/bin/bash
DISK_IMG="disk.img"
SHARE="./src/module"

qemu-system-x86_64 -hda "$DISK_IMG" -nographic \
-virtfs local,path="$SHARE",mount_tag=shared_folder,security_model=passthrough,id=host0
