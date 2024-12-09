qemu-system-x86_64 \
-m 1G \
-kernel linux-6.11/arch/x86/boot/bzImage \
-hda wheezy.img \
-serial stdio \
-append "root=/dev/sda console=ttyAMA0 console=ttyS0 nokaslr" \
-virtfs local,path=my_shared_folder,mount_tag=hostshare,security_model=mapped \
-s

