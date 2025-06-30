#!/bin/sh

# install packages for Gentoo Linux

# these two are optional
emerge -v app-portage/eix app-portage/gentoolkit
emerge -v app-editors/vim

# chapel dependencies
emerge -v dev-vcs/git dev-libs/gmp dev-python/pip dev-python/virtualenv


#source /etc/profile
#echo "sys-devel/clang $@" >/etc/portage/package.use/llvm
#emerge -v sys-devel/clang:11
