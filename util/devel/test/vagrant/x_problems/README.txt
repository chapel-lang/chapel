2018-09-05: Problems with netbsd-NetBSD-7.0

On a known-good Ubuntu 16.04 workstation, I installed Virtualbox and Vagrant
from apt-get, and successfully built and ran Chapel's current set of Vagrant
tests starting from scratch.  Everything worked first time except for
netbsd-NetBSD-7.0. That's why I moved that subdirectory here.
- I had to install Ubuntu's NFS server packages on the Ubuntu 16.04 host
  system before the netbsd-NetBSD-7.0 Vagrant box would even start.
- Then, the provision script git-clone-chapel.sh errored because no git
  client was found in PATH on the Vagrant box.
- I ssh'd to the Vagrant box and installed git by hand, using the pkgin
  commandline found in netbsd-pkgin-deps.sh.  When I ran "sudo bash" to
  be able to run the installer, "bash" was not found. So I also installed
  bash as well as git. The git client did not seem to be installed after the
  first try, so I repeated the pkgin commands. Suspect the netbsd-pkgin-deps.sh
  script may have other problems.
- Then, the provision script git-clone-chapel.sh errored again because the
  git client had "SSL certificate problems". That's where I stopped.
