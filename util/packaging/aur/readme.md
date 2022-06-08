
This folder contains manual copies of Arch User Repository (AUR) packaging (one AUR package per directory).
The official remote sources for each of these are `https://aur.archlinux.org/packages/<AUR package name>`.
To clone the remote git repo hosted by AUR and containing the packaging and build files `git clone https://aur.archlinux.org/<AUR package name>.git`

To test these builds:
- install the `base-devel` package using (`sudo pacman -S base-devel`) since dependence on packages included in `base-devel` is not checked; this need only be done once
- change directories to AUR package you would like to build (e.g. `cd chapel-git`).
- build ready-to-install package from source using `makepkg --syncdeps`, which also installs any required dependencies
- install the local package with `makepkg --install`, you will be prompted for the root password

To bump to the next version number:
- ensure you have maintainer priveleges to the AUR git repo and clone it (address given above)
- (in `PKGBUILD`) update the version number and checksum (and dependencies if needed)
- generate the `.SRCINFO` from `PKGBUILD` using `makepkg --printsrcinfo > .SRCINFO`
- `add`, `commit`, and `push` your changes

For more detailed info about the purpose of the variables and functions in `PKGBUILD` see the articles on [`PKGBUILD`](https://wiki.archlinux.org/title/PKGBUILD) and [`makepkg`](https://wiki.archlinux.org/title/Makepkg) from the Arch Wiki.
[This article](https://wiki.archlinux.org/title/Creating_packages) contains a more general overview of packaging for Arch.
