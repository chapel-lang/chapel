Testing Chapel with Vagrant

This directory is meant to help with testing Chapel with Vagrant. It
contains scripts and Vagrantfiles.

== Pre-requisites ==

You'll need to install Vagrant and VirtualBox to use this directory.
(You might be able to get another VM solution to work... but that's on you)

== About <distribution>/Vagrantfile, boxes, and VMs ==

The Vagrantfiles are each in a directory named in an obvious way from the
available vagrant VM. For example, the Vagrant VM available as
bento/centos-6.8-i386 is requested in the Vagrantfile in the directory
bento-centos-6.8-i386.

If you are unfamiliar with Vagrant, reading up on it a bit before trying
to use this directory is probably a good idea. Each Vagrantfile is just
some instructions for how to set up a VM. The VMs themselves are stored
in two places:
 * vagrant boxes (e.g. run `vagrant box list`)
 * VirtualBox VMs (or whatever) - for me, these end up in:
   ~/VirtualBox\ VMs

Some of these Vagrantfiles assume you are using VirtualBox to run the
VMs. While Vagrant supports other options, VirtualBox is the most
supported / most recommended and the boxes (such as bento and ubuntu
images that the Vagrantfiles specify to download) usually start with
VirtualBox support.

== About the scripts in this directory ==

The scripts in this directory automate common tasks that amount to doing
the same thing across all the VMs / distributions. I expect that simply

./chapelquickcheck.sh

and

./chapeldefaultcheck.sh

will be sufficient for basic testing, but the other scripts are there for
debugging and doing other investigations. Each script summarizes its
purpose in comments near the top, and most take no arguments.

== About the scripts in provision-scripts ==

The scripts in provision-scripts are ones that Vagrant will use when
setting up these VMs, as directed in the Vagrantfiles. For example,
there is a script to use apt-get to install Chapel dependencies.

which Vagrant will use when setting up
VMs, as directed in Vagrantfiles.

