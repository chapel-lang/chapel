#!/bin/sh

package_ver="22.1.8-1"
el_os_ver="$(. /etc/os-release; echo "$PLATFORM_ID" | cut -d: -f2)"
ver_string="-$package_ver.$el_os_ver"

dnf -y install llvm-devel$ver_string clang$ver_string clang-devel$ver_string
