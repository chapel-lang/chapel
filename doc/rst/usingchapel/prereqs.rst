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

  * You have a Bourne shell available at ``/bin/sh``, 'env' available at
    ``/usr/bin/env``, and that 'env' can locate ``python3`` or ``python``
    on your system.

  * You have Python 2.7 or newer.

  * You have access to gmake or a GNU-compatible version of make.

  * You have access to standard C and C++14 compilers. The C++14 support
    is required for building the compiler itself. For GCC specifically,
    GCC 5 or newer is required for C++14 support. Note that C11 support,
    while not required, will enable faster atomic operations.

  * The LLVM backend is now the default and it is easiest to use it with
    a system-wide installation of LLVM. Only LLVM 11/12 are currently
    supported. If a system-wide installation of LLVM 11/12 is not available,
    you can use the bundled LLVM or disable LLVM support (see
    :ref:`readme-chplenv.CHPL_LLVM`). Please note that building the
    bundled LLVM requires cmake version 3.13.4 or later.

In addition, several optional components have additional requirements:

  * Python 3.7 or newer is required if you want to use chpldoc, c2chapel,
    or Chapel's test system. These additionally require ``python3-devel``
    or the equivalent package; ``python3`` and ``pip3`` commands; and the
    ``venv`` Python package.

  * ``cmake`` 3.13.4 or later is required to build the bundled LLVM or
    to build the complete documentation

  * ``doxygen`` is required to build the complete documentation

  * ``m4`` is required for building the bundled GMP

  * ``git`` is required for :ref:`readme-mason`, chapel's package manager

  * ``pkg-config`` is required for the ``mason system`` subcommands


.. _readme-prereqs-installation:

Installation
------------

We have used the following commands to install the above prerequisites:

  * CentOS, Fedora::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake llvm-devel clang clang-devel

  * Debian, Ubuntu::

      sudo apt-get install gcc g++ m4 perl python3 python3-pip python3-venv python3-dev bash make mawk git pkg-config cmake llvm-12-dev llvm-12 llvm-12-tools clang-12 libclang-12-dev libclang-cpp12-dev libedit-dev

  * FreeBSD::

     sudo pkg install gcc m4 perl5 python3 bash gmake gawk git pkgconf cmake llvm12
