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
    ./util/devel/test/portability/apptainer/extract-docs.py > doc/rst/usingchapel/prereqs-commands.rst
  Then make any adjustments to add any notes

We have used the following commands to install the above prerequisites:

.. include:: prereqs-commands.rst

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
