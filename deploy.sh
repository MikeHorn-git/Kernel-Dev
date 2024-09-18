#! /bin/bash

wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.11.tar.xz
tar -xvf linux-6.11.tar.xz
rm linux-6.11.tar.xz
cd linux-6.11
make defconfig
