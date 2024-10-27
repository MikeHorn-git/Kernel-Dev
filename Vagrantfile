# -*- mode: ruby -*-
# vi: set ft=ruby :
# frozen_string_literal: true

require 'getoptlong'

opts = GetoptLong.new(
  ['--kernel', GetoptLong::OPTIONAL_ARGUMENT]
)

kernel = false

opts.ordering = (GetoptLong::REQUIRE_ORDER)

opts.each do |opt, _arg|
  case opt
  when '--kernel'
    kernel = true
  end
end

Vagrant.configure('2') do |config|
  ENV['LC_ALL'] = 'en_US.UTF-8'
  config.vm.box = 'generic/alpine318'
  config.vm.box_version = '4.3.12'

  config.vm.hostname = 'host'
  config.vm.synced_folder '.', '/vagrant'

  config.vm.provider 'virtualbox' do |vb|
    vb.memory = '4096'
    vb.cpus = 4
    vb.name = 'Kernel'
    vb.gui = false
    vb.check_guest_additions = false
  end

  config.vm.provision 'shell', inline: <<-SHELL, privileged: false
    sudo apk update
    sudo apk upgrade
    sudo apk add bison build-base elfutils-dev flex libressl-dev linux-headers perl wget xz
  SHELL

  if kernel
    config.vm.provision 'shell', inline: <<-SHELL, privileged: false
      KERNEL_VERSION="6.11"
      wget --no-verbose https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-${KERNEL_VERSION}.tar.xz
      tar -xf linux-${KERNEL_VERSION}.tar.xz
      cd linux-${KERNEL_VERSION}

      make defconfig
      make
      sudo make modules_install
      sudo make install

      sudo mkinitfs -o /boot/initramfs 6.11.0/
      sudo sed -i -e 's|^  LINUX vmlinuz-virt|  LINUX vmlinuz|' -e 's|^  INITRD initramfs-virt|  INITRD initramfs|' /boot/extlinux.conf
      sudo reboot
    SHELL
  end
end
