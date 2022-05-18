
This folder contains manual copies of Arch User Repository (AUR) packaging (one AUR package per directory).
The official remote sources for each of these are `https://aur.archlinux.org/packages/<AUR package name>`.

To test these builds:
- install the `base-devel` package using (`sudo pacman -S base-devel`) since dependence on packages included in `base-devel` is not checked; this need only be done once
- change directories to AUR package you would like to build (e.g. `cd chapel-git`).
- build ready-to-install package from source using `makepkg --syncdeps`, which also installs any required dependencies
- install the local package with `makepkg --install`, you will be prompted for the root password
