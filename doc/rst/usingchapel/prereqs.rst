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

  * You have Python 3.5 or newer available as either ``python3`` or
    ``python`` and that ``env`` can locate it.

  * You have access to ``gmake`` or a GNU-compatible version of ``make``.

  * You have access to standard C and C++ compilers.

    * Building the Chapel compiler and bundled components requires
      C++17 and one of the following:

      * GCC 7.4 or newer

      * LLVM Clang 14.0 or newer

      * Apple Xcode/Clang 11.0 or newer

    * C11 support, while not required, will enable faster atomic operations.

  * CMake is available and ``cmake`` runs version 3.20 or later.

  * The LLVM backend is now the default and it is easiest to use it with a
    system-wide installation of LLVM and clang. LLVM versions 14 through 20 are
    currently supported. If a system-wide installation of LLVM and clang with
    one of those versions is not available, you can use the bundled LLVM or
    disable LLVM support (see :ref:`readme-chplenv.CHPL_LLVM`).

  * When using the LLVM installation bundled with Chapel
    (``CHPL_LLVM=bundled``), make sure your machine has at least 4GB of memory
    available before building Chapel. If you are using your system's LLVM
    (``CHPL_LLVM=system``), Chapel can be built with at least 2GB of memory.
    See :ref:`readme-chplenv.CHPL_LLVM` for more information about these
    settings. Note that the memory requirements for building Chapel may vary
    in other configurations or for parallel builds.

In addition, several optional components have additional requirements:

  * Python 3.10 or newer is required if you want to use chpldoc, c2chapel,
    chapel-py, chplcheck, chpl-language-server, or Chapel's test system. These
    additionally require ``python3-devel``
    or the equivalent package; ``python3`` and ``pip3`` commands; and the
    ``venv`` Python package.

  * ``doxygen`` 1.8.17 or newer is required to build the complete documentation

  * ``m4`` is required for building the bundled GMP

  * ``git`` is required for :ref:`readme-mason`, chapel's package manager

  * ``pkg-config`` is required for the ``mason system`` subcommands


.. _readme-prereqs-installation:

Installation
------------

.. comment:

  The commands below are automatically generated.
  To regenerate them:
    cd util/devel/test/portability/apptainer
    ./extract-docs.py
    paste output below & adjust to add any notes

We have used the following commands to install the above prerequisites:

  * Alma Linux 8, 9.6, 10.0::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Alpine 3.18, 3.19::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake
      sudo apk add llvm15-dev clang15-dev llvm15-static clang15-static


  * Alpine 3.20, 3.21::

      sudo apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake
      sudo apk add llvm-dev clang-dev clang-static llvm-static


  * Amazon Linux 2 (but note `Amazon Linux 2 CHPL_LLVM==system incompatibility`_)::

      sudo yum install git gcc gcc-c++ m4 perl python tcsh bash perl python python-devel python-setuptools bash make gawk python3 which
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
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install clang clang-devel llvm-devel


  * Arch::

      sudo pacman -Syu
      sudo pacman -S base-devel
      sudo pacman -S cmake git python
      sudo pacman -S llvm clang


  * CentOS Stream 9, 10::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Debian 11 "Bullseye" (but note `Newer CMake required to build LLVM`_)::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-16-dev llvm-16 llvm-16-tools clang-16 libclang-16-dev libclang-cpp16-dev libedit-dev


  * Debian 12 "Bookworm", 13 "Trixie"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev


  * Fedora 41, 42::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * FreeBSD 13.5, 14.2::

      sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake
      sudo pkg install llvm


  * OpenSuse Leap 15.6::

      sudo zypper install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake
      sudo zypper install llvm-devel clang-devel clang


  * Rocky Linux 8.10, 9.5::

      sudo dnf upgrade
      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake
      sudo dnf install which diffutils
      sudo dnf install llvm-devel clang clang-devel


  * Ubuntu 22.04 "Jammy Jellyfish", 24.04 "Noble Numbat", 24.10 "Oracular Oriole", 25.04 "Plucky Puffin"::

      sudo apt-get update
      sudo apt-get install gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
      sudo apt-get install llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev

Compatibility Notes
-------------------

Amazon Linux 2 CHPL_LLVM==system incompatibility
++++++++++++++++++++++++++++++++++++++++++++++++

Amazon Linux 2 uses GCC 7.3.1 and only provides LLVM 11, but Chapel requires a
newer GCC and newer LLVM. To use Chapel on this platform, installing a newer
GCC is required. The repositories provide a GCC 10 package, which can be used
to configure Chapel.

.. code-block:: bash

    export CC=gcc10-gcc
    export CXX=gcc10-g++
    export CHPL_HOST_CC=gcc10-gcc
    export CHPL_HOST_CXX=gcc10-g++

Chapel can then be built with ``CHPL_LLVM=none`` (still requires the newer GCC)
or ``CHPL_LLVM=bundled``.

Newer CMake required to build LLVM
++++++++++++++++++++++++++++++++++

On some systems, the cmake package is not new enough to build the bundled
LLVM. That can be addressed either by installing CMake from source or by
installing a system LLVM package using the commands shown above.

Note that the LLVM support library is used even with ``CHPL_LLVM=none``,
and so installing a system LLVM on these platforms is still important in
that case.
