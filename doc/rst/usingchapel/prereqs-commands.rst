  * Alma Linux 10, 8, 9::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Alpine 3.21, 3.22, 3.23::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake libunwind-dev coreutils
      sudo apk add llvm-dev clang-dev clang-static llvm-static


  * Amazon Linux 2023::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install clang clang-devel llvm-devel


  * Arch::

      sudo pacman -Syu
      sudo pacman -S base-devel
      sudo pacman -S cmake git python libunwind
      sudo pacman -S llvm clang


  * CentOS Stream 10, 9::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Debian 11 "Bullseye" (but see note `Newer CMake required to build LLVM`_)::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-16-dev llvm-16 llvm-16-tools clang-16 libclang-16-dev libclang-cpp16-dev libedit-dev


  * Debian 12 "Bookworm"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-16-dev llvm-16 llvm-16-tools clang-16 libclang-16-dev libclang-cpp16-dev libedit-dev


  * Debian 13 "Trixie"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


  * Fedora 43, 44::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * FreeBSD 13.5, 14.3 (but see note `Outdated FreeBSD testing`_)::

      sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake libunwind
      sudo pkg install llvm


  * OpenSuse Leap 16.0::

      sudo zypper install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake libunwind-devel
      sudo zypper install findutils diffutils
      sudo zypper install llvm-devel clang-devel clang


  * Rocky Linux 10, 8, 9::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Ubuntu 22.04 "Jammy Jellyfish"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-15-dev llvm-15 llvm-15-tools clang-15 libclang-15-dev libclang-cpp15-dev libedit-dev


  * Ubuntu 24.04 "Noble Numbat", 25.10 "Questing Quokka", 26.04 "Resolute Raccoon"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


Compatibility Notes
-------------------

Outdated FreeBSD testing
++++++++++++++++++++++++

Our portability testing for FreeBSD relies on public Vagrant boxes. At time of
writing (May 2026), we have been unable to find a box for FreeBSD releases
newer than 14.3. Due to limited resources, and lacking information on how
widely used Chapel is on FreeBSD, we have not taken on the work of making our
own box or otherwise continuing to update this test coverage. It is still our
intention to support FreeBSD as a best effort, so feel free to open bug reports
for Chapel on FreeBSD versions newer than we test, and/or let us know if this
lack of testing coverage causes you concern.

Newer CMake required to build LLVM
++++++++++++++++++++++++++++++++++

On some systems, the cmake package is not new enough to build the bundled
LLVM. That can be addressed either by installing CMake from source or by
installing a system LLVM package using the commands shown above.

Note that the LLVM support library is used even with ``CHPL_LLVM=none``,
and so installing a system LLVM on these platforms is still important in
that case.
