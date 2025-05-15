#!/bin/sh

mkdir -p /usr/local/etc/pkg/repos
echo 'FreeBSD: { url: "https://pkg.FreeBSD.org/${ABI}/quarterly", mirror_type: "none" }' > /usr/local/etc/pkg/repos/FreeBSD.conf

pkg install --yes gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake
