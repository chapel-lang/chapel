#!/bin/sh

# Adjustment needed to reliably work on network restricted test machines
# (FreeBSD 15+ renamed the FreeBSD repo to FreeBSD-ports)

mkdir -p /usr/local/etc/pkg/repos
echo 'FreeBSD-ports: { url: "https://pkg.FreeBSD.org/${ABI}/quarterly", mirror_type: "none" }' > /usr/local/etc/pkg/repos/FreeBSD.conf
