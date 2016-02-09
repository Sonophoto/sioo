# -*- mode: ruby -*-
# vi: set ft=ruby :

# Steps to use this software in Vagrant
# 1) Install VirtualBox
# 2) Install Vagrant
# 3) In project directory run 'vagrant up'
# 4) vagrant ssh
# 5) cd /SiOO
# 6) Learn more at www.vagrantup.com

$script = <<SCRIPT
echo "Building.."
cd /SiOO
bash make-sioo
SCRIPT

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"
  config.vm.synced_folder ".", "/vagrant", disabled: true
  config.vm.synced_folder ".", "/SiOO"
  config.vm.provision "shell", inline: $script
end
