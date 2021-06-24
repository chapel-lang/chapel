.. _readme-prereqs:

====================
Chapel Prerequisites
====================

If you have a standard UNIX environment, a C/C++ compiler, some basic
scripting languages, a GNU-compatible make, and awk installed you should
have no problems getting started with Chapel.


Prerequisites
-------------

In slightly more detail, the following are prerequisites and assumptions
about your environment for using Chapel:

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

  * Building GMP requires an M4 macro processor.

  * Building LLVM requires cmake version 3.13.4 or later.

  * If you wish to use chpldoc or Chapel's test system, Python 3.6 or
    newer is required and the ``python3`` and ``pip3`` commands must be
    available. The ``venv`` Python package must be available (note that
    it is often included by default in a Python 3 installation).
    Additionally, ``curl``, ``perl``, and ``python3-devel``
    (or equivalent packages for your platform) are required.

  * If you wish to use :ref:`readme-mason`, chapel's package manager,
    ``git`` is required.

    * The ``mason system`` subcommands additionally require ``pkg-config``.

  * The LLVM backend is now the default and it is easiest to use it with
    a system-wide installation of LLVM. Only LLVM 11 is currently
    supported. If a system-wide installation of LLVM 11 is not available,
    you can use the bundled LLVM or disable LLVM support (see
    :ref:`readme-chplenv.CHPL_LLVM`).

.. _readme-prereqs-installation:

Installation
------------

We have used the following commands to install the above prerequisites:

  * CentOS, Fedora::

      sudo dnf install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git

  * Debian, Ubuntu::

      sudo apt-get install gcc g++ m4 perl python3 python3-pip python3-venv python3-dev bash make mawk git pkg-config llvm-11-dev llvm-11 llvm-11-tools clang-11 libclang-11-dev libedit-dev

  * FreeBSD::

     sudo pkg install gcc m4 perl5 python3 py37-pip bash gmake gawk git pkgconf
