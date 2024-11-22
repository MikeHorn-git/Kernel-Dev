# frozen_string_literal: true

Vagrant.configure('2') do |config|
  ENV['LC_ALL'] = 'en_US.UTF-8'
  config.vm.box = 'generic/alpine318'
  config.vm.box_version = '4.3.12'

  config.vm.hostname = 'host'
  config.vm.synced_folder '.', '/vagrant', disabled: true
  config.vm.provision 'file', source: './module',
                              destination: '/home/vagrant/module'

  config.vm.provider 'virtualbox' do |vb|
    vb.memory = '4096'
    vb.cpus = 4
    vb.name = 'Kernel'
    vb.gui = false
    vb.check_guest_additions = false
    vb.customize ['modifyvm', :id, '--clipboard', 'bidirectional']
  end

  config.vm.provision 'shell', inline: <<-SHELL, privileged: false
    # Upgrade Alpine from 3.18 to 3.20
    sudo sed -i -e 's/v3\.18/v3\.20/g' /etc/apk/repositories

    # Add & Upgrade packages
    sudo apk upgrade -U -a --ignore linux-headers linux-virt
    sudo apk add bison build-base elfutils-dev flex git htop libressl-dev linux-headers neofetch perl strace wget xz

    # Omb
    bash -c "$(curl -fsSL https://raw.githubusercontent.com/ohmybash/oh-my-bash/master/tools/install.sh)"

    # Add & enable rsyslog for kernel logs
    sudo service rsyslog start
    sudo rc-update add rsyslog default

    ## Bashrc tweaks ##
    # Set CTRL+L to clear
    echo 'bind -x '"'"'"\C-l":clear'"'"'' >> ~/.bashrc
    # Avoid 'xterm-kitty': unknown terminal type
    echo "TERM=xterm-256color" >> ~/.bashrc
  SHELL

  if ENV['VAGRANT_KERNEL'] == 'true'
    config.vm.provision 'shell', inline: <<-SHELL, privileged: false
      # Download Kernel
      KERNEL_VERSION="6.11"
      wget --no-verbose https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-${KERNEL_VERSION}.tar.xz
      tar -xf linux-${KERNEL_VERSION}.tar.xz
      cd linux-${KERNEL_VERSION}

      # Configure & Install Kernel
      make defconfig
      make
      sudo make modules_install
      sudo make install

      # Create an initramfs with new Kernel
      sudo mkinitfs -o /boot/initramfs 6.11.0/

      # Update bootloader
      sudo sed -i -e 's|^  LINUX vmlinuz-virt|  LINUX vmlinuz|' -e 's|^  INITRD initramfs-virt|  INITRD initramfs|' /boot/extlinux.conf
      sudo reboot
    SHELL
  end
end
