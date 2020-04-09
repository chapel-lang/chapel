#!/bin/sh

pkgin -y install gcc6 perl5 python27 py27-setuptools bash gmake gawk git-base pkgconf mozilla-rootcerts

/usr/pkg/sbin/mozilla-rootcerts install

# There has got to be a better way to do this.
if [ ! -f /usr/pkg/bin/python ]
then
  ln -s /usr/pkg/bin/python2.7 /usr/pkg/bin/python
fi
