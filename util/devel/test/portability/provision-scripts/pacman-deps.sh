#!/bin/sh

# Note: you can search arch packages with
#   pacman -Ss search-term
# You can update the system and packages list with
#  pacman -Syu

# update to avoid 404 errors
pacman --noconfirm -Syu

# install base development tools
pacman --noconfirm -S base-devel

# install additional packages
pacman --noconfirm -S cmake git python
