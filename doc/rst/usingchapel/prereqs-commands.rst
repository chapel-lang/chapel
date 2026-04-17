  * Alma Linux 10, 8, 9::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Alpine 3.20, 3.21, 3.22, 3.23::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake libunwind-dev coreutils
      sudo apk add llvm-dev clang-dev clang-static llvm-static


  * Amazon Linux 2 (but note `Amazon Linux 2 CHPL_LLVM==system incompatibility`_)::

      sudo yum install git gcc gcc-c++ m4 perl python tcsh bash perl python python-devel python-setuptools bash make gawk python3 which libunwind-devel
      sudo yum install wget tar openssl-devel
      wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
      tar xvzf cmake-3.25.1.tar.gz
      cd cmake-3.25.1
      ./bootstrap
      make
      sudo make install
      sudo update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
      sudo yum install gcc10 gcc10-c++
      export CC=gcc10-gcc
      export CXX=gcc10-g++
      export CHPL_HOST_CC=gcc10-gcc
      export CHPL_HOST_CXX=gcc10-g++
      export CHPL_LLVM=bundled


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


  * CentOS Stream 9, 10::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Debian 11 "Bullseye"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-16-dev llvm-16 llvm-16-tools clang-16 libclang-16-dev libclang-cpp16-dev libedit-dev


  * Debian 12 "Bookworm", 13 "Trixie"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


  * Fedora 42, 43::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * FreeBSD 13.5, 14.3::

      sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake libunwind
      sudo pkg install llvm


  * OpenSuse Leap 15.6, 16.0::

      sudo zypper install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake libunwind-devel
      sudo zypper install findutils diffutils
      sudo zypper install llvm-devel clang-devel clang


  * Rocky Linux 10, 8, 9::

      sudo dnf upgrade
      sudo dnf install epel-release
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Ubuntu 22.04 "Jammy Jellyfish", 24.04 "Noble Numbat", 25.10 "Questing Quokka"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake libunwind-dev
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


