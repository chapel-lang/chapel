#!/bin/sh

# Adjustment needed to reliably work on network restricted test machines

mkdir -p /usr/local/etc/pkg/repos
echo 'FreeBSD: { url: "https://pkg.FreeBSD.org/${ABI}/quarterly", mirror_type: "none" }' > /usr/local/etc/pkg/repos/FreeBSD.conf
