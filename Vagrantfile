# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty32"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = "1024"
    vb.gui = true
  end

  config.vm.provision :shell, :inline => 'export DEBIAN_FRONTEND=noninteractive'
  config.ssh.shell = "bash -c 'BASH_ENV=/etc/profile exec bash'"
  config.vm.provision :shell, :inline => 'echo "cd /vagrant;" >> /home/vagrant/.bashrc'
  
  # Install xfce and virtualbox additions
  config.vm.provision "shell", inline: "sudo apt-get update"
  config.vm.provision "shell", inline: "sudo apt-get install -y xfce4 virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11"
  # Permit anyone to start the GUI
  config.vm.provision "shell", inline: "sudo sed -i 's/allowed_users=.*$/allowed_users=anybody/' /etc/X11/Xwrapper.config"
end
