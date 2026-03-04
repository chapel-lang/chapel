.. _readme-riscv:

======================
Using Chapel on RISC-V
======================

This document describes how to run Chapel on a standalone RISC-V machine.
Chapel and its build system work as expected in this setting, but only specific choices for the library dependencies work.

.. contents::

-------------------
Building for RISC-V
-------------------

One working example for building Chapel on RISC-V is:

.. code-block:: bash

    export CHPL_COMM=none
    export CHPL_TARGET_MEM=mimalloc
    export CHPL_HOST_MEM=mimalloc
    export CHPL_LLVM=bundled
    export CHPL_LLVM_TARGETS_TO_BUILD=host
    ./configure
    make -j

The mimalloc and cstdlib allocators are confirmed to work, jemalloc currently does not.
To learn more about the different alloocators available in Chapel, see :ref:`readme-chplenv.CHPL_HOST_MEM` and :ref:`readme-chplenv.CHPL_TARGET_MEM`.
Using mimalloc (instead of cstdlib) and enabling LLVM is expected to give better performance.
This particular example builds the bundled copy of LLVM included with the Chapel source, but the mechanisms for using another installation of LLVM will also work.
See :ref:`readme-chplenv.CHPL_LLVM` for details on how to do that.
The various communication layers are untested and may or may not work.
The ``CHPL_LLVM_TARGETS_TO_BUILD`` controls which LLVM backends are built and is used here simply to reduce the build time.
