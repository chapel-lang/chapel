.. _readme-prereqs:

====================
Chapel Prerequisites
====================

Chapel is designed to be portable to a variety of systems and only
requires software that is common on UNIX systems.

The following are prerequisites and assumptions about your environment
for using Chapel:

  * You are using an environment that supports standard UNIX commands
    such as: ``cd, mkdir, rm, echo``

  * You have a Bourne shell available at ``/bin/sh`` and ``env`` available at
    ``/usr/bin/env``.

  * You have Python 2.7 or newer available as either ``python3`` or
    ``python`` and that ``env`` can locate it.

  * You have access to ``gmake`` or a GNU-compatible version of ``make``.

  * You have access to standard C and C++ compilers.

    * Building the Chapel compiler and bundled components requires
      C++14 and one of the following:

      * GCC 7.1 or newer

      * Clang 5.0 or newer

      * Apple Clang 9.3 or newer

    * C11 support, while not required, will enable faster atomic operations.

  * CMake is available and ``cmake`` runs version 3.13.4 or later.

  * The LLVM backend is now the default and it is easiest to use it with a
    system-wide installation of LLVM and clang. On Mac OS X, LLVM 14 and
    15 are supported. On other platforms, LLVM and clang versions 11 through 15
    are currently supported. If a system-wide installation of
    LLVM and clang with one of those versions is not available, you can
    use the bundled LLVM or disable LLVM support (see
    :ref:`readme-chplenv.CHPL_LLVM`).

In addition, several optional components have additional requirements:

  * Python 3.7 or newer is required if you want to use chpldoc, c2chapel,
    or Chapel's test system. These additionally require ``python3-devel``
    or the equivalent package; ``python3`` and ``pip3`` commands; and the
    ``venv`` Python package.

  * ``doxygen`` 1.8.17 or newer is required to build the complete documentation

  * ``m4`` is required for building the bundled GMP

  * ``git`` is required for :ref:`readme-mason`, chapel's package manager

  * ``pkg-config`` is required for the ``mason system`` subcommands

  * ``cmake`` 3.16 or newer is required to install ``chpl`` when choosing an
    installation with ``./configure --chpl-home=/path/to/install``


.. _readme-prereqs-installation:

Installation
------------

.. comment:

  The commands below are automatically generated.
  To regenerate them:
    cd util/devel/test/singularity
    ./extract-docs.py
    paste output below

We have used the following commands to install the above prerequisites:

  * Alma Linux 8, 9.0, 9.1, 9.2::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Alpine 3.15, 3.17::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake
      sudo apk add llvm-dev clang-dev clang-static llvm-static


  * Alpine 3.18::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake
      sudo apk add llvm15-dev clang15-dev llvm15-static clang15-static


  * Amazon Linux 2::

      sudo yum install git gcc gcc-c++ m4 perl python tcsh bash gcc gcc-c++ perl python python-devel python-setuptools bash make gawk python3 which
      sudo yum install wget tar openssl-devel
      wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
      tar xvzf cmake-3.25.1.tar.gz
      cd cmake-3.25.1
      ./bootstrap
      make
      sudo make install
      sudo update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
      sudo yum install llvm-devel clang clang-devel


  * Amazon Linux 2023::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install clang clang-devel llvm-devel


  * Arch::

      sudo pacman -Syu
      sudo pacman -S base-devel
      sudo pacman -S cmake git python
      sudo pacman -S llvm14 clang14


  * CentOS 7 Devtoolset 11::

      sudo yum install centos-release-scl
      sudo yum install devtoolset-11-gcc*
      sudo yum install epel-release
      sudo scl enable devtoolset-11 bash
      sudo echo source scl_source enable devtoolset-11 >> ~/.bashrc
      sudo yum install git gcc gcc-c++ m4 perl tcsh bash gcc gcc-c++ perl python3 bash make gawk cmake3
      sudo echo export CMAKE=cmake3 >> ~/.bashrc


  * CentOS Stream 8::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * CentOS Stream 9::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel-15.0.7 clang-15.0.7 clang-devel-15.0.7


  * Debian 10 "Buster"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-13-dev llvm-13 llvm-13-tools clang-13 libclang-13-dev libclang-cpp13-dev libedit-dev


  * Debian 12 "Bookworm", 11 "Bullseye"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


  * Fedora 34, 35, 36, 37::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * FreeBSD 12.2, 12.4, 13.1::

      sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake
      sudo pkg install llvm13


  * FreeBSD 13.2::

      sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake
      sudo pkg install llvm


  * OpenSuse Leap 15.3, 15.4, 15.5::

      sudo zypper install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake
      sudo zypper install llvm-devel clang-devel clang


  * Rocky Linux 8, 9.0, 9.1, 9.2::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Ubuntu 20.04 "Focal Fossa"::

      sudo apt-get update
      sudo apt-get install software-properties-common
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-12-dev llvm-12 llvm-12-tools clang-12 libclang-12-dev libclang-cpp12-dev libedit-dev


  * Ubuntu 22.04 "Jammy Jellyfish"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


  * Ubuntu 22.10 "Kinetic Kudu"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-14-dev llvm-14 llvm-14-tools clang-14 libclang-14-dev libclang-cpp14-dev libedit-dev
